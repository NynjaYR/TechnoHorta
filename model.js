import moongoose from "mongoose";

const buttonstatusschema = new moongoose.Schema({
  /*button1: String,
  moment: Number,*/
  temperatura: String,
  umidade: String,
  umidade_solo_A: String,
  umidade_solo_B: String,
  umidade_solo_C: String
});

export default moongoose.model("btstats", buttonstatusschema);