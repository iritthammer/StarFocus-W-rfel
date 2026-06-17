const express = require('express');
const cors = require('cors');

const app = express();

app.use(express.json());
app.use(cors());

let currentSide = "Default";

app.post('/update', (req, res) => {

    currentSide = req.body.side;

    console.log("Neue Seite:", currentSide);

    res.send("OK");
});

app.get('/current-side', (req, res) => {
    res.json({ side: currentSide });
});

app.listen(3000, () => {
    console.log("Server läuft auf Port 3000");
});