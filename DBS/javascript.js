let httpServer = "localhost:8888";
const MAX_VALUES_SHOW = 30;

const States = Object.freeze({
    AUTOMATIC: "AUTOMATIC",
    MANUAL: "MANUAL",
    UNCONNECTED: "UNCONNECTED",
    NOT_AVAILABLE: "NOT AVAILABLE"
});

let currentState = States.NOT_AVAILABLE;

window.addEventListener("load", function () {
    this.document.querySelector("#openModalButton").click();
    createChart();
    updateCusStatus();
});

function setServerAddress() {
    httpServer = document.querySelector("#serverAddress").value;
}

let xValues = ["2025-02-05 12:00", "2025-02-05 14:00", "2025-02-05 12:00", "2025-02-05 14:00","2025-02-05 12:00", "2025-02-05 14:00","2025-02-05 12:00", "2025-02-05 14:00","2025-02-05 12:00", "2025-02-05 14:00","2025-02-05 12:00", "2025-02-05 14:00","2025-02-05 12:00", "2025-02-05 14:00","2025-02-05 12:00", "2025-02-05 14:00","2025-02-05 12:00", "2025-02-05 14:00","2025-02-05 12:00", "2025-02-05 14:00","2025-02-05 12:00", "2025-02-05 14:00"];
let yValues = [7,8, 7,8, 7,8, 7,8, 7,8, 7,8, 7,8, 7,8, 7,8, 7,8, 7,8];

async function createChart() {

    try {
        const response = await fetch("http://" + httpServer + "/api/data");
        if (!response.ok) {
            throw new Error(response.status);
        }
        const json = await response.json();
        parseChart(json);
    } catch (e) {
        console.log(e.message);
        cusNotReachable();
    }
    setTimeout(createChart, 2000);
}

function parseChart(json) {
    let xValues = [];
    let yValues = [];

    for (let i = json.length > MAX_VALUES_SHOW ? json.length - MAX_VALUES_SHOW : 0; i < json.length; i++) {
        xValues.push(json[i].timestamp);
        yValues.push(json[i].value);
    }

    new Chart("myChart", {
        type: "line",
        data: {
            labels: xValues,
            datasets: [{
                fill: false,
                backgroundColor: "rgba(0,0,255,1.0)",
                borderColor: "rgba(0,0,255,0.5)",
                data: yValues
            }]
        },
        options: {
            animation: false,
            legend: { display: false },
            maintainAspectRatio: false,
            scales: {
                yAxes: [{
                    scaleLabel: {
                        display: true,
                        labelString: 'Distance [cm]'
                    }
                }]
            }
        }
    });
}

function cusNotReachable() {
    currentState = States.NOT_AVAILABLE;
    document.querySelector("#cusState").innerHTML = currentState;
    document.querySelector("#valveLevel").innerHTML = "--";
    document.querySelector("#switchAuto").classList.add("d-none");
    document.querySelector("#switchMan").classList.add("d-none");
}

async function updateCusStatus() {

    try {
        const response = await fetch("http://" + httpServer + "/api/status");
        if (!response.ok) {
            throw new Error(response.status);
        }
        const json = await response.json();
        parseStatus(json);
    } catch (e) {
        console.log(e.message);
        cusNotReachable();
    }

    setTimeout(updateCusStatus, 1000);
}

function parseStatus(json) {
    switch (json.status) {
        case "AUTO":
            currentState = States.AUTOMATIC;
            document.querySelector("#switchAuto").classList.add("d-none");
            document.querySelector("#switchMan").classList.remove("d-none");
            document.querySelector("#valveLevel").innerHTML = json.opening;
            break;

        case "MANUAL":
            document.querySelector("#switchAuto").classList.remove("d-none");
            document.querySelector("#switchMan").classList.add("d-none");
            document.querySelector("#valveLevel").innerHTML = json.opening;
            document.querySelector("#valveOpening").value = json.opening;
            currentState = States.MANUAL;
            break;

        case "UNCONNECTED_AUTO":
        case "UNCONNECTED_MANUAL":
            document.querySelector("#switchAuto").classList.add("d-none");
            document.querySelector("#switchMan").classList.add("d-none");
            document.querySelector("#valveLevel").innerHTML = "--";
            currentState = States.UNCONNECTED;
            break;

        default:
            console.log(json);
    }

    document.querySelector("#cusState").innerHTML = currentState;
}

async function toggleMode() {
    if (currentState == States.AUTOMATIC || currentState == States.MANUAL) {
        try {
            const response = await fetch("http://" + httpServer + "/api/toggle", {method: "POST"});
            if (!response.ok) {
                throw new Error(response.status);
            }
            updateCusStatus();
        } catch (e) {
            console.log(e.message);
            cusNotReachable();
        }
    }
}

async function setValveOpening() {
    if (currentState == States.MANUAL) {
        try {
            const formData = new FormData();
            formData.append("value", document.querySelector("#valveOpening").value);
            const response = await fetch("http://" + httpServer + "/api/opening", {
                method: "POST",
                body: JSON.stringify(Object.fromEntries(formData))
            });
            if (!response.ok) {
                throw new Error(response.status);
            }
            updateCusStatus();
        } catch (e) {
            console.log(e.message);
            cusNotReachable();
        }
    }
}