import { useState, useuseEffect } from "react";
import reactLogo from "./assets/react.svg";
import viteLogo from "/vite.svg";
import thLogo from "/TH.png";
import "./App.css";
import "../api.js";

function getRandomIntInclusive(min, max) {
  min = Math.ceil(min);
  max = Math.floor(max);
  return Math.floor(Math.random() * (max - min + 1)) + min;
}

function App() {

//

  const [statusBotao, setStatusBotao] = useState([]);

  useEffect(() => {
    // Pegue a URL pública da porta 3000 na aba 'Ports' do Codespaces
    const URL_DO_CODESPACES = 'https://fantastic-computing-machine-x6vvq567xg26w9q-3000.app.github.dev/';

    // O React faz uma "ligação" para a rota /btstats do seu api.js
    fetch(`${URL_DO_CODESPACES}/btstats`)
      .then(resposta => resposta.json())
      .then(dados => {
        setStatusBotao(dados); // Salva o array do Mongo no React
      })
      .catch(erro => console.error("Erro ao conectar no api.js:", erro));
  }, []);

//

  const [count, setCount] = useState(0);

  return (
    <>
      <div>
        <a href="https://youtu.be/dQw4w9WgXcQ" target="_blank">
          <img src={thLogo} className="logo" alt="TH logo" />
        </a>
      </div>
      <h1>TechnoHorta</h1>
      <div className="card">
        <button
          onClick={() => setCount((count) => getRandomIntInclusive(1, 20))}
        >
          roll a D20: {count}
        </button>
        <p>Just working on login page now :)</p>
      </div>
      <p className="read-the-docs">Don't click on the logo</p>
            <br/>
            <h1>Status dos Botões (Vindo do Mongo)</h1>
      <ul>
        {statusBotao.map((item) => (
          <li key={item._id}>
            Botão: {item.button1} | Momento: {item.moment}
          </li>
        ))}
      </ul>
    </>
  );
}

export default App;
