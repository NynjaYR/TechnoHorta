import React, { useState, useEffect } from 'react';

function App() {
  const [contatos, setContatos] = useState([]);
  const [carregando, setCarregando] = useState(true);

  const [isLigado, setIsLigado] = useState(false);

    const alternarDispositivo = () => {
    // comando baseado no estado atual
    const textoSimples = isLigado ? 'rele_off_A' : 'rele_on_A';

    fetch('http://localhost:3000/api/mqtt-publish', {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({
        topico: 'pin_state', 
        mensagem: textoSimples
      })
    })
    .then(res => {
      if (!res.ok) throw new Error('Erro na resposta do servidor');
      return res.json();
    })
    .then(data => {
      if (data.sucesso) {
        setIsLigado(!isLigado); 
      } else {
        alert(' Falha ao enviar para o MQTT.');
      }
    })
    .catch(err => {
      console.error("Erro na rota MQTT:", err);
      alert(' Erro de conexão com a API.');
    });
  };

  //END OF MQTT

  useEffect(() => {
    // 1. Criamos a função que faz o fetch na sua API local
    const buscarDadosDoBanco = () => {
      fetch('http://localhost:3000/btstats')
        .then((response) => {
          if (!response.ok) throw new Error('Erro ao conectar na API');
          return response.json();
        })
        .then((data) => {
          setContatos(data);    // Atualiza o estado com os dados novos do Mongo
          setCarregando(false); // Desativa o texto de carregamento na primeira vez
        })
        .catch((error) => {
          console.error("Erro na busca automática:", error);
          setCarregando(false);
        });
    };

    // 2. Chame a função imediatamente para a tela não abrir em branco
    buscarDadosDoBanco();

    // 3. Configura o timer para rodar a função sozinha a cada 3 segundos (3000ms)
    const temporizador = setInterval(buscarDadosDoBanco, 3000);

    // 4. Limpeza obrigatória do React para evitar vazamento de memória se mudar de página
    return () => clearInterval(temporizador);
  }, []); // Mantém o array vazio para o timer ser criado apenas uma vez

  // Tela de carregamento inicial
  if (carregando) {
    return <p style={{ fontStyle: 'italic', padding: '20px' }}>Conectando ao banco de dados...</p>;
  }

  return (
  <div 
    style={{ 
      display: 'flex', 
      flexDirection: 'column', 
      justifyContent: 'center', 
      alignItems: 'center',     
      minHeight: '100vh',       
      width: '100vw',            
      boxSizing: 'border-box',   
      margin: '0',               
      position: 'absolute',
      top: 0,
      left: 0
    }}
  >
   <div 
      style={{ 
        padding: '30px', 
        borderRadius: '8px', 
        boxShadow: '0 4px 6px rgba(0,0,0,0.1)',
        maxWidth: '500px',
        width: '100%'
      }}
    >

      
      {contatos.length === 0 ? (
        <p>Nenhum registro encontrado no MongoDB.</p>
      ) : (
<ul style={{ listStyleType: 'none', padding: 0 }}>
  {contatos
    .slice(-5) //
    .reverse() //
    .map((item) => (
      <li 
        key={item._id} 
        style={{ 
          padding: '12px', 
          borderBottom: '1px solid #ddd', 
          marginBottom: '5px',
          borderRadius: '4px'
        }}
      >
        <strong>Temp:</strong> {item.temperatura} | <strong>Umid:</strong> {item.umidade} | <strong>Solo A:</strong> {item.umidade_solo_A} | <strong>Solo B:</strong> {item.umidade_solo_B} | <strong>Solo C:</strong> {item.umidade_solo_C}
      </li>
  ))}
</ul>
      )}

      <h1 style={{ marginTop: 0, fontSize: '22px', color: '#333' }}>Interruptor MQTT</h1>
        <p style={{ color: '#666', fontSize: '14px', marginBottom: '25px' }}>
          Clique para alternar o estado do dispositivo.
        </p>

        <button
          onClick={alternarDispositivo}
          style={{
            width: '100%',
            padding: '16px',
            // Verde se estiver ligado (#28a745), Vermelho se estiver desligado (#dc3545)
            backgroundColor: isLigado ? '#28a745' : '#dc3545', 
            color: 'white',
            border: 'none',
            borderRadius: '6px',
            fontSize: '16px',
            fontWeight: 'bold',
            cursor: 'pointer',
            boxShadow: '0 2px 4px rgba(0,0,0,0.2)',
            transition: 'background-color 0.3s ease' // Suaviza a troca de cor
          }}
        >
          {isLigado ? 'RELÉ: ATIVADO (Clique para Desligar)' : 'RELÉ: DESATIVADO (Clique para Ligar)'}
        </button>
    </div>
    </div>
  );
}

export default App;