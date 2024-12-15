const express = require('express');
const bodyParser = require('body-parser');
const app = express();

// Middleware untuk memparsing JSON
app.use(bodyParser.json());

// Variabel untuk menyimpan data suhu, kelembapan, dan waktu
let temperature = null;
let humidity = null;
let time = null;

// Endpoint untuk menerima data dari ESP32
app.post('/data', (req, res) => {
  const data = req.body;
  temperature = data.temperature;
  humidity = data.humidity;
  time = data.time;
  console.log('Data received from ESP32:', data);
  res.status(200).send({ message: 'Data received successfully' });
});

// Endpoint untuk menyediakan data suhu, kelembapan, dan waktu
app.get('/data', (req, res) => {
  if (temperature !== null && humidity !== null && time !== null) {
    res.status(200).json({
      temperature: temperature,
      humidity: humidity,
      time: time
    });
  } else {
    res.status(404).send({ message: 'Data not available' });
  }
});

// Menjalankan server pada port 3000
app.listen(3000, () => {
  console.log('Server running on http://localhost:3000');
});