import express from "express";
import dotenv from "dotenv";
import mongoose from "mongoose";

dotenv.config();

const app = express();
const PORT = 3000;

const connectdb = async () => {
  await mongoose.connect(process.env.MONGO_URI);
  console.log(`conectado ao db`);
};

app.get("/", (req, res) => {});

app.listen(PORT, () => console.log(`servidor rodando na porta ${PORT}`));
