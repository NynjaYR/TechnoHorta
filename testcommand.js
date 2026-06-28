import moongoose from "mongoose";

const buttonstatusschema = new moongoose.Schema({
  button1: String,
  moment: Number,
});

export default moongoose.model("btstats", buttonstatusschema);