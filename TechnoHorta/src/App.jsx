import { useState } from 'react'
import reactLogo from './assets/react.svg'
import viteLogo from '/vite.svg'
import thLogo from '/TH.png'
import './App.css'

function getRandomIntInclusive(min, max) {
  min = Math.ceil(min);
  max = Math.floor(max);
  return Math.floor(Math.random() * (max - min + 1)) + min; 
}

function App() {
  const [count, setCount] = useState(0)

  return (
    <>
      <div>
        <a href="https://youtu.be/dQw4w9WgXcQ" target="_blank">
          <img src={thLogo} className="logo" alt="TH logo" />
        </a>
      </div>
      <h1>TechnoHorta</h1>
      <div className="card">
        <button onClick={() => setCount((count) => getRandomIntInclusive(1, 20))}>
          roll a D20: {count}
        </button>
        <p>
          Edit <code>src/App.jsx</code> and save to test HMR
        </p>
      </div>
      <p className="read-the-docs">
        Don't click on the logo
      </p>
    </>
  )
}

export default App
