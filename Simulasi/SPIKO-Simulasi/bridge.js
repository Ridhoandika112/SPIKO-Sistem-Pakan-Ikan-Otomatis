const { SerialPort } = require('serialport');
const { ReadlineParser } = require('@serialport/parser-readline');
const WebSocket = require('ws');

const SERIAL_PORT = 'COM6';
const BAUD_RATE   = 9600;
const WS_PORT     = 8081;

const wsClients = new Set();
const wss = new WebSocket.Server({ port: 8081 });
console.log('[*] WebSocket Server berjalan di ws://127.0.0.1:8081');

wss.on('connection', (ws) => {
    console.log('[+] HTML Dashboard terhubung.');
    wsClients.add(ws);

    ws.on('message', (message) => {
        const cmd = message.toString().trim();
        console.log(`[PERINTAH WEB -> ARDUINO] ${cmd}`);
        if (port && port.isOpen) {
            port.write(cmd + '\n', (err) => {
                if (err) console.error('[!] Gagal kirim ke serial:', err.message);
            });
        } else {
            console.log('[!] Serial port belum terbuka.');
        }
    });

    ws.on('close', () => {
        wsClients.delete(ws);
        console.log('[-] HTML Dashboard terputus.');
    });
});

const port = new SerialPort({
    path: SERIAL_PORT,
    baudRate: BAUD_RATE,
    autoOpen: false
});

const parser = port.pipe(new ReadlineParser({ delimiter: '\n' }));

port.open((err) => {
    if (err) {
        console.error(`[!] Gagal buka ${SERIAL_PORT}:`, err.message);
        return;
    }
    console.log(`[+] Serial port ${SERIAL_PORT} terbuka @ ${BAUD_RATE} baud`);
});

parser.on('data', (line) => {
    line = line.trim();
    if (!line) return;
    console.log(`[WOKWI -> WEB] ${line}`);

    wsClients.forEach((ws) => {
        if (ws.readyState === WebSocket.OPEN) {
            ws.send(line);
        }
    });
});

port.on('error', (err) => console.error('[SERIAL ERROR]', err.message));
port.on('close', () => console.log('[!] Serial port tertutup.'));
