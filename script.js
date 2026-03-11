let data;
let schedules = [];

const days = {
    "pon":0,
    "uto":1,
    "sre":2,
    "cet":3,
    "pet":4
};

fetch("timetable.json")
.then(r => r.json())
.then(j => {
    data = j.predmeti;
});

function slot(vreme)
{
    let d = vreme.substring(0,3);
    let h = parseInt(vreme.substring(3));
    return days[d]*100 + h;
}

function conflict(schedule, termin)
{
    for (let t of termin.termini)
    {
        let s = slot(t.vreme);
        if (schedule.has(s)) return true;
    }
    return false;
}

function addTermin(schedule, termin, predmet, tip)
{
    let ns = new Map(schedule);

    for (let t of termin.termini)
    {
        let s = slot(t.vreme);

        ns.set(s,{
            predmet: predmet,
            tip: tip,
            vreme: t.vreme,
            soba: t.soba
        });
    }

    return ns;
}

function getSelections()
{
    let result = [];

    data.forEach(predmet => {

        let c = document.querySelector(
            "input[name=\"" + predmet.naziv + "_c\"]"
        );

        if (!c || !c.checked) return;

        let p = document.querySelector(
            "select[name=\"" + predmet.naziv + "_p\"]"
        )?.value;

        let v = document.querySelector(
            "select[name=\"" + predmet.naziv + "_v\"]"
        )?.value;

        let k = document.querySelector(
            "select[name=\"" + predmet.naziv + "_k\"]"
        )?.value;

        result.push({ predmet, p, v, k });

    });

    return result;
}

function buildSubjects(i, selections, schedule)
{
    if (i === selections.length)
    {
        schedules.push(schedule);
        return;
    }

    let sel = selections[i];

    let activities = [];

    if (sel.predmet.p && sel.p !== "-")
        activities.push({tip:"p", list:sel.predmet.p, choice:sel.p});

    if (sel.predmet.v && sel.v !== "-")
        activities.push({tip:"v", list:sel.predmet.v, choice:sel.v});

    if (sel.predmet.k && sel.k !== "-")
        activities.push({tip:"k", list:sel.predmet.k, choice:sel.k});

    buildActivities(0, activities, schedule, sel.predmet.naziv, selections, i);
}

function buildActivities(j, activities, schedule, predmet, selections, subjectIndex)
{
    if (j === activities.length)
    {
        buildSubjects(subjectIndex + 1, selections, schedule);
        return;
    }

    let act = activities[j];

    act.list.forEach(t => {

        if (act.choice !== "0" && act.choice !== t.nastavnik)
            return;

        if (conflict(schedule, t))
            return;

        let ns = addTermin(schedule, t, predmet, act.tip);

        buildActivities(j+1, activities, ns, predmet, selections, subjectIndex);

    });
}

function generate()
{
    schedules = [];

    let selections = getSelections();

    buildSubjects(0, selections, new Map());

    showSchedules();
}

function showSchedules()
{
    let out = document.getElementById("results");

    out.innerHTML = "";

    if (schedules.length === 0)
    {
        out.innerHTML = "No valid schedules.";
        return;
    }

    schedules.forEach((s,i) => {

        let div = document.createElement("div");

        let title = document.createElement("h3");
        title.innerText = "Schedule " + (i+1);

        div.appendChild(title);

        let list = document.createElement("ul");

        for (let [k,v] of s)
        {
            let li = document.createElement("li");

            li.innerText =
                v.predmet + " (" + v.tip + ") " +
                v.vreme + " " +
                v.soba;

            list.appendChild(li);
        }

        div.appendChild(list);

        out.appendChild(div);

    });
}