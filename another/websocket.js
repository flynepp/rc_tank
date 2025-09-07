function createWebSocket(ip, port) {
    const ws = new WebSocket(`ws://${ip}:${port}/`);
    ws.binaryType = 'blob';

    ws.onopen = () => {
        producer(`WebSocket connected: ${ip}:${port}`);
    };

    ws.onmessage = (event) => {
        messageHandle(event);
    };

    ws.onclose = () => {
        producer(`WebSocket closed: ${ip}:${port}`);
    };
    ws.onerror = (err) => producer(`WebSocket error: ${ip}:${port}`);

    return ws;
}

function messageHandle(event) {
    if (event.data instanceof Blob) {
        imgShow(event.data);
    } else if (typeof event.data === "string") {
        messageShow(event.data);
    }
}

function imgShow(data) {
    const url = URL.createObjectURL(data);
    img.onload = () => {
        URL.revokeObjectURL(url);
        img.onload = null;

        img.style.transform = 'rotate(' + rotation + 'deg)';
        img.style.transformOrigin = 'center center';
    };
    img.src = url;
}

function messageShow(data) {
    const info = JSON.parse(data);

    cpuInfo.innerText = `${info.cpu.toFixed(2)}%`;
    memInfo.innerText = `${info.mem.toFixed(2)}%`;
}