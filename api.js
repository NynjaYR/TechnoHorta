import express from "express";
import dotenv from "dotenv";
import mongoose from "mongoose";
import cors from 'cors';
import btstats from "./testcommand.js";

dotenv.config();

const app = express();
const PORT = 3000;  

app.use(express.json());
app.use(cors());

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
    const buttonstatusatual = await btstats.create(req.body);
    res.json(buttonstatusatual);
  }
  catch (error) {req.send(error)}
});

//Get

app.get("/btstats", async (req, res) => {
  try {
    const buttonstatusatual = await btstats.find();
    res.json(buttonstatusatual);
  }
  catch (error) {res.send(error)}
});

app.listen(PORT, () => console.log(`servidor rodando na porta ${PORT}`));
