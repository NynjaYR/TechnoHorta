import express from "express";
import dotenv from "dotenv";
import mongoose from "mongoose";
import cors from 'cors';
import mqtt from "mqtt";
import btstats from "./model.js";

dotenv.config();

const app = express();
const PORT = 3000;  

app.use(express.json());
app.use(cors());

//MQTT
const mqttClient = mqtt.connect('mqtt://192.168.3.6:1883');
mqttClient.on('connect', () => {
    console.log('Conectado com sucesso ao Broker MQTT!');
});

app.post('/api/mqtt-publish', (req, res) => {
    const { mensagem, topico } = req.body;

    const topicoAlvo = topico || 'pin_state';
    const payloadText = String(mensagem);

    mqttClient.publish(topicoAlvo, payloadText, (err) => {
        if (err) {
            console.error('Erro ao publicar no MQTT:', err);
            return res.status(500).json({ sucesso: false, erro: err.message });
        }
        
        console.log(`Texto enviado para [${topicoAlvo}]:`, payloadText);
        res.json({ sucesso: true, mensagem: "Texto publicado no MQTT!" });
    });
});
//MQTT END

//console.log(process.env.MONGO_URI)

const connectdb = async () => {
  try {
    await mongoose.connect(process.env.MONGO_URI);
    console.log(`conectado ao db`);
  } catch (error) {
    console.log(`erro ao tentar conectar no db`, error);
  }
};

connectdb();

//Create

app.post("/btstats", async (req, res) => {
  try {
    const novobuttonstatusatual = await btstats.create(req.body);
    return res.json(novobuttonstatusatual); 
  }
  catch (error) {
    console.error("Erro no processamento do POST:", error);
    return res.status(400).send(error.message); 
  }
});

//Get

app.get("/btstats", async (req, res) => {
  try {
    const buttonstatusatual = await btstats.find();
    res.json(buttonstatusatual);
  }
  catch (error) {res.send(error)}
});

//Update

app.put('/btstats/:id', async (req, res) => {
  try {
    const novobuttonstatusatual = await btstats.findByIdAndUpdate(req.params.id, req.body, { new: true });
    res.json(novobuttonstatusatual);
  }
  catch (error) {res.send(error)}
});

//Delete

app.delete('/btstats/:id', async (req, res) => {
  try {
    const novobuttonstatusatualdeleted = await btstats.findByIdAndDelete(req.params.id);
    res.json(novobuttonstatusatualdeleted);
  }
  catch (error) {res.send(error)}
});

//Send to http port 3000
app.listen(PORT, () => console.log(`servidor rodando na porta ${PORT}`));
