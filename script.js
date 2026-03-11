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
.then(j => data = j.predmeti);

function getSelections() {

    let result = [];

    data.forEach(predmet => {

        let checkbox = document.querySelector(
            "input[name='" + predmet.naziv + "_c']"
        );

        if (!checkbox || !checkbox.checked)
            return;

        let p = document.querySelector(
            "select[name='" + predmet.naziv + "_p']"
        )?.value;

        let v = document.querySelector(
            "select[name='" + predmet.naziv + "_v']"
        )?.value;

        let k = document.querySelector(
            "select[name='" + predmet.naziv + "_k']"
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

function slot(vreme) {

    let d = vreme.substring(0,3);
    let h = parseInt(vreme.substring(3));

    return days[d]*100 + h;
}

function conflict(schedule, termin) {

    for (let t of termin.termini) {

        let s = slot(t.vreme);

        if (schedule.has(s))
            return true;
    }

    return false;
}

function addTermin(schedule, termin, predmet, tip) {

    let newSched = new Map(schedule);

    for (let t of termin.termini) {

        let s = slot(t.vreme);

        newSched.set(s,{
            predmet: predmet,
            tip: tip,
            soba: t.soba
        });
    }

    return newSched;
}

function build(i, selections, schedule) {

    if (i === selections.length) {

        schedules.push(schedule);
        return;
    }

    let sel = selections[i];

    tryTip("p", sel.p, sel.predmet.p);
    
    tryTip("k", sel.k, sel.predmet.k);

    function tryTip(tip, teacherChoice, list) {

        if (!list || teacherChoice === "-")
            return;

        list.forEach(t => {

            if (teacherChoice !== "0" &&
                teacherChoice !== t.nastavnik)
                return;

            if (conflict(schedule, t))
                return;

            let ns = addTermin(
                schedule,
                t,
                sel.predmet.naziv,
                tip
            );
            
            if (tip === "p")
                tryTip("v", teacherChoice, list);
            if (tip === "v")
                tryTip("k", teacherChoice, list);
            if (tip === "k")
                build(i+1, selections, ns);
        });

    }

}

function generate() {

    schedules = [];

    let selections = getSelections();

    build(0, selections, new Map());

    showSchedules();
}

function showSchedules() {

    let out = document.getElementById("results");

    out.innerHTML = "";

    schedules.forEach((s, i) => {

        let div = document.createElement("div");

        div.innerHTML = "<h3>Schedule "+(i+1)+"</h3>";

        s.forEach((v,k) => {

            let d = Math.floor(k/100);
            let h = k%100;

            div.innerHTML +=
                ["pon","uto","sre","cet","pet"][d] +
                " " + h +
                " - " + v.predmet +
                " ("+v.tip+") " +
                v.soba +
                "<br>";
        });

        out.appendChild(div);
    });

}