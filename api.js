import express from "express";
import dotenv from "dotenv";
import mongoose from "mongoose";

dotenv.config();

const app = express();
const PORT = 3000;

const connectdb = async () => {
  try {
    await mongoose.connect(process.env.MONGO_URI);
    console.log(`conectado ao db`);
  } catch (error) {
    console.log(`erro ao tentar conectar no db`, error);
  }
};

connectdb();

app.post("/TechnoHorta", (req, res) => {});

//app.get("/", (req, res) => {});

app.listen(PORT, () => console.log(`servidor rodando na porta ${PORT}`));
