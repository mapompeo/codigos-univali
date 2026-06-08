import conectarAoBanco from "../config/dbConfig.js";
import { ObjectId } from "mongodb";

const conexao = await conectarAoBanco(process.env.STRING_CONEXAO);

export const getPosts = async () => {
   const db = conexao.db("instavale");
   const colecao = db.collection("posts");
   return colecao.find().toArray();
}

export const getPostById = async (id) => {
    if (!ObjectId.isValid(id)) {
        return null; // ID inválido
    }
   const db = conexao.db("instavale");
   const colecao = db.collection("posts");
   return colecao.findOne({ _id: new ObjectId(id) });
}

export const createPost = async (novoPost) => {
   const db = conexao.db("instavale");
   const colecao = db.collection("posts");
   return colecao.insertOne(novoPost);
}

export const updatePost = async (id, dadosAtualizados) => {
   const db = conexao.db("instavale");
   const colecao = db.collection("posts");
   return colecao.updateOne({ _id: new ObjectId(id) }, { $set: dadosAtualizados });
}

export const deletePost = async (id) => {
   const db = conexao.db("instavale");
   const colecao = db.collection("posts");
   return colecao.deleteOne({ _id: new ObjectId(id) });
}