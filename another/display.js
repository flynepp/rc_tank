const infoDisplay = document.querySelector("#left-display");

const infos = [
    "test connect",
    "server started",
    "user logged in",
    "error: connection timeout",
    "warning: low battery",
    "upload complete",
    "download failed",
    "retrying...",
    "system shutdown",
    "reboot success",
    "Error: timeout.",
    "Reboot success.",
    "This is a very long message that should automatically wrap into multiple lines depending on the container width."
];

const queue = [];

async function consumer() {
    async function typeLine(text) {
        const line = document.createElement("div");
        infoDisplay.appendChild(line);

        for (let i = 0; i < text.length; i++) {
            line.textContent += text[i];
            await new Promise(r => setTimeout(r, 20));
        }

        checkOverflow();
    }

    function checkOverflow() {
        while (infoDisplay.children.length > 15) {
            infoDisplay.removeChild(infoDisplay.firstChild);
        }
    }

    function sleep(ms) {
        return new Promise(resolve => setTimeout(resolve, ms));
    }

    while (true) {
        if (queue.length > 0) {
            const info = queue.shift();
            await typeLine(info);
        } else {
            await sleep(100);
        }
    }
}

function producer(info) {
    queue.push(info);
}

function getRandomInfo() {
    const length = infos.length;

    const i = Math.floor(Math.random() * length);

    return infos[i];
}

consumer();
setInterval(() => {
    producer("Info: " + getRandomInfo());
}, 1000);
