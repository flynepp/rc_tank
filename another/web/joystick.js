const LeftJoystick = nipplejs.create({
    zone: document.getElementById('left-joystick'),
    mode: 'static',
    position: { left: '50%', top: '50%' },
    color: 'gray',
    size: 150
});

let currentX = 0, currentY = 0;

LeftJoystick.on('move', function (evt, data) {
    if (data.vector) {
        let x = data.vector.x;
        let y = data.vector.y;

        setCurrent(x, y);
    }
});

LeftJoystick.on('end', function () {
    currentX = 0;
    currentY = 0;
});

const RigthJoystick = nipplejs.create({
    zone: document.getElementById('right-joystick'),
    mode: 'static',
    position: { left: '50%', top: '50%' },
    color: 'gray',
    size: 150,
    lockX: true,
    lockY: false
});

let currentZ = 0;

RigthJoystick.on('move', function (evt, data) {
    if (data.vector) {
        let x = data.vector.x;

        currentZ = Math.abs(x) > 0.2 ? x : 0;
    }
});

RigthJoystick.on('end', function () {
    currentZ = 0;
});

setInterval(() => {
    if (ws) {
        ws.send(`["${currentX.toFixed(2)}, ${currentY.toFixed(2)}", "${currentZ.toFixed(2)}"]`);
    }
}, 50);

//////////////////////////////////////////////////////////////////

function setCurrent(x, y) {
    // 输入归一化，保证 x^2 + y^2 <= 1
    let len = Math.sqrt(x * x + y * y);
    if (len > 1) {
        x /= len;
        y /= len;
    }

    // 阈值死区
    currentX = Math.abs(x) > 0.2 ? x : 0;
    currentY = Math.abs(y) > 0.2 ? y : 0;
}

//////////////////////////////////////////////////////////////////

const maxKeys = 2;
const keysPressed = new Set();

const keyMap = {
    'w': 'up',
    'a': 'left',
    's': 'down',
    'd': 'right',
    'arrowleft': 'left',
    'arrowright': 'right'
};

window.addEventListener('keydown', (e) => {
    const key = e.key.toLowerCase();

    if (!keyMap[key]) return;

    if (keysPressed.size >= maxKeys) return;

    if (keysPressed.has(key)) return;

    keysPressed.add(key);

    updateMovement();
});

window.addEventListener('keyup', (e) => {
    const key = e.key.toLowerCase();
    if (!keyMap[key]) return;

    keysPressed.delete(key);
    updateMovement();
});

function updateMovement() {
    const pressed = Array.from(keysPressed);

    let x = 0, y = 0, z = 0;
    if (pressed.includes('w')) y += 1;
    if (pressed.includes('s')) y -= 1;
    if (pressed.includes('a')) x -= 1;
    if (pressed.includes('d')) x += 1;
    if (pressed.includes('arrowleft')) z -= 1;
    if (pressed.includes('arrowright')) z += 1;

    currentZ = z;
    setCurrent(x, y);
    setJoystickPosition(x, y, LeftJoystick);
    setJoystickPosition(z, 0, RigthJoystick);
}

function setJoystickPosition(x, y, joystick) {
    const nipple = joystick;
    if (!nipple || !nipple[0]) return;

    const stickEl = nipple[0].ui.front;
    const size = nipple[0].options.size / 2;

    const px = x * size;
    const py = -y * size;

    stickEl.style.transform = `translate(${px}px, ${py}px)`;
}