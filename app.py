from flask import Flask, render_template, request
import json

app = Flask(__name__)

with open("timetable.json", encoding="utf8") as f:
    data = json.load(f)["predmeti"]

days = {
    "pon": 0,
    "uto": 1,
    "sre": 2,
    "cet": 3,
    "pet": 4
}


def slot(vreme):
    d = vreme[:3]
    h = int(vreme[3:])
    return days[d] * 100 + h


def conflict(schedule, termin):

    for t in termin["termini"]:
        s = slot(t["vreme"])

        if s in schedule:
            return True

    return False


def add_termin(schedule, termin, predmet, tip):

    ns = schedule.copy()

    for t in termin["termini"]:
        s = slot(t["vreme"])

        ns[s] = {
            "predmet": predmet,
            "tip": tip,
            "vreme": t["vreme"],
            "soba": t["soba"]
        }

    return ns


def generate_schedules(selections):

    schedules = []

    def build_subject(i, schedule):

        if i == len(selections):
            schedules.append(schedule)
            return

        predmet, activities = selections[i]

        def build_activity(j, schedule2):

            if j == len(activities):
                build_subject(i + 1, schedule2)
                return

            tip, options = activities[j]

            for termin in options:

                if conflict(schedule2, termin):
                    continue

                ns = add_termin(schedule2, termin, predmet, tip)

                build_activity(j + 1, ns)

        build_activity(0, schedule)

    build_subject(0, {})

    return schedules


@app.route("/")
def index():
    return render_template("index.html")


@app.route("/generate", methods=["POST"])
def generate():

    form = request.form
    selections = []

    for predmet in data:

        name = predmet["naziv"]

        # checkbox not checked
        if name + "_c" not in form:
            continue

        activities = []

        for tip in ["p", "v", "k"]:

            if tip not in predmet:
                continue

            choice = form.get(name + "_" + tip)

            if choice == "-":
                continue

            options = []

            for t in predmet[tip]:

                if choice != "0" and t["nastavnik"] != choice:
                    continue

                options.append(t)

            if options:
                activities.append((tip, options))

        selections.append((name, activities))

    schedules = generate_schedules(selections)

    return render_template("result.html", schedules=schedules)


if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000)
