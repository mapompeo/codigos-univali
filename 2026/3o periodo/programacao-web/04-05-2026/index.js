const express = require('express');
const app = express();
const port = 3000;

app.use(express.json());

const catalogo = [
  {
    id: 1,
    titulo: "Interstellar",
    ano: 2014,
    genero: "Ficção Científica",
    imagem: "https://link-generico.com/interstellar.jpg",
    resumo: "Uma equipe de exploradores viaja através de um buraco de minhoca no espaço."
  },
  {
    id: 2,
    titulo: "Breaking Bad",
    ano: 2008,
    genero: "Drama",
    imagem: "https://link-generico.com/breaking-bad.jpg",
    resumo: "Um professor de química se torna um mestre do crime."
  },
  {
    id: 3,
    titulo: "Spider-Man: Across the Spider-Verse",
    ano: 2023,
    genero: "Animação",
    imagem: "https://link-generico.com/spiderman.jpg",
    resumo: "Miles Morales viaja pelo multiverso."
  },
  {
    id: 4,
    titulo: "Attack on Titan",
    ano: 2013,
    genero: "Anime",
    imagem: "https://link-generico.com/aot.jpg",
    resumo: "A humanidade luta contra gigantes."
  }
];

app.get('/itens', (req, res) => {
  res.json(catalogo);
});

app.get('/item/:id', (req, res) => {
  const { id } = req.params;
  const item = catalogo.find(i => i.id === parseInt(id));
  
  if (item) {
    res.json(item);
  } else {
    res.status(404).send("Item não encontrado.");
  }
});

app.get('/item/ano/:ano', (req, res) => {
  const { ano } = req.params;
  const resultado = catalogo.filter(i => i.ano === parseInt(ano));
  res.json(resultado);
});

app.get('/item/:inicio/:fim', (req, res) => {
  const { inicio, fim } = req.params;
  const resultado = catalogo.filter(i => 
    i.ano >= parseInt(inicio) && i.ano <= parseInt(fim)
  );
  res.json(resultado);
});

app.get('/item/genero/:genero', (req, res) => {
  const { genero } = req.params;
  const resultado = catalogo.filter(i => 
    i.genero.toLowerCase() === genero.toLowerCase()
  );
  res.json(resultado);
});

app.listen(port, () => {
  console.log(`UniFlix rodando em http://localhost:${port}`);
});