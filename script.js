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
    console.log("JSON loaded");
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

        if (schedule.has(s))
            return true;
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

        let checkbox =
            document.querySelector("input[name=\"" + predmet.naziv + "_c\"]");

        if (!checkbox || !checkbox.checked)
            return;

        let p = document.querySelector(
            "select[name=\"" + predmet.naziv + "_p\"]"
        )?.value;

        let v = document.querySelector(
            "select[name=\"" + predmet.naziv + "_v\"]"
        )?.value;

        let k = document.querySelector(
            "select[name=\"" + predmet.naziv + "_k\"]"
        )?.value;

        result.push({
            predmet,
            p,
            v,
            k
        });

    });

    return result;
}

function subjectOptions(sel)
{
    let options = [[]];

    function process(type, teacherChoice, list)
    {
        if (!list) return;

        if (teacherChoice === "-")
            return;

        let newOptions = [];

        list.forEach(t => {

            if (teacherChoice !== "0" &&
                teacherChoice !== undefined &&
                teacherChoice !== t.nastavnik)
                return;

            options.forEach(o => {
                newOptions.push([...o,{
                    tip:type,
                    termin:t
                }]);
            });

        });

        options = newOptions;
    }

    process("p", sel.p, sel.predmet.p);
    process("v", sel.v, sel.predmet.v);
    process("k", sel.k, sel.predmet.k);

    if (options.length === 0)
        options = [[]];

    return options;
}

function build(i, selections, schedule)
{
    if (i === selections.length)
    {
        schedules.push(schedule);
        return;
    }

    let sel = selections[i];
    let options = subjectOptions(sel);

    options.forEach(opt => {

        let sched = new Map(schedule);
        let bad = false;

        opt.forEach(item => {

            if (conflict(sched, item.termin))
                bad = true;
            else
                sched = addTermin(
                    sched,
                    item.termin,
                    sel.predmet.naziv,
                    item.tip
                );

        });

        if (!bad)
            build(i+1, selections, sched);

    });
}

function generate()
{
    schedules = [];

    let selections = getSelections();

    console.log("Selections:", selections);

    build(0, selections, new Map());

    console.log("Schedules:", schedules.length);

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
                v.predmet +
                " (" + v.tip + ") " +
                v.vreme +
                " " + v.soba;

            list.appendChild(li);
        }

        div.appendChild(list);

        out.appendChild(div);

    });
}