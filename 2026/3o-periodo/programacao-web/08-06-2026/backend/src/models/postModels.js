import conectarAoBanco from "../config/dbConfig.js";
import { ObjectId } from "mongodb";
import fs from "fs/promises";

const conexao = await conectarAoBanco(process.env.STRING_CONEXAO);
const storageUrl = new URL("../data/posts.json", import.meta.url);

async function lerPostsArquivo() {
   try {
      const dados = await fs.readFile(storageUrl, "utf-8");
      return JSON.parse(dados);
   } catch {
      return [];
   }
}

async function salvarPostsArquivo(posts) {
   await fs.mkdir(new URL("../data", import.meta.url), { recursive: true });
   await fs.writeFile(storageUrl, JSON.stringify(posts, null, 2));
}

function usarMongo() {
   return Boolean(conexao);
}

export const getPosts = async () => {
   if (usarMongo()) {
      const db = conexao.db("instavale");
      const colecao = db.collection("posts");
      return colecao.find().toArray();
   }

   return lerPostsArquivo();
}

export const getPostById = async (id) => {
    if (!ObjectId.isValid(id)) {
        return null; // ID inválido
    }

   if (usarMongo()) {
      const db = conexao.db("instavale");
      const colecao = db.collection("posts");
      return colecao.findOne({ _id: new ObjectId(id) });
   }

   const posts = await lerPostsArquivo();
   return posts.find((post) => post._id === id) || null;
}

export const createPost = async (novoPost) => {
   if (usarMongo()) {
      const db = conexao.db("instavale");
      const colecao = db.collection("posts");
      return colecao.insertOne(novoPost);
   }

   const posts = await lerPostsArquivo();
   const postComId = {
      ...novoPost,
      _id: novoPost._id ? novoPost._id.toString() : new ObjectId().toHexString()
   };
   posts.push(postComId);
   await salvarPostsArquivo(posts);
   return { acknowledged: true, insertedId: postComId._id };
}

export const updatePost = async (id, dadosAtualizados) => {
   if (usarMongo()) {
      const db = conexao.db("instavale");
      const colecao = db.collection("posts");
      return colecao.updateOne({ _id: new ObjectId(id) }, { $set: dadosAtualizados });
   }

   const posts = await lerPostsArquivo();
   const indice = posts.findIndex((post) => post._id === id);

   if (indice === -1) {
      return { matchedCount: 0, modifiedCount: 0 };
   }

   posts[indice] = { ...posts[indice], ...dadosAtualizados, _id: id };
   await salvarPostsArquivo(posts);
   return { matchedCount: 1, modifiedCount: 1 };
}

export const deletePost = async (id) => {
   if (usarMongo()) {
      const db = conexao.db("instavale");
      const colecao = db.collection("posts");
      return colecao.deleteOne({ _id: new ObjectId(id) });
   }

   const posts = await lerPostsArquivo();
   const postsFiltrados = posts.filter((post) => post._id !== id);

   if (postsFiltrados.length === posts.length) {
      return { deletedCount: 0 };
   }

   await salvarPostsArquivo(postsFiltrados);
   return { deletedCount: 1 };
}