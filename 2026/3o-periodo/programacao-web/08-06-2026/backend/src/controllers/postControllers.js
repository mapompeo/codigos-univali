import { getPosts, getPostById, createPost, updatePost, deletePost } from "../models/postModels.js";
import { ObjectId } from "mongodb";
import fs from "fs";

export const controllerPostPost = async (req, res) => {
    const postReq = req.body;
    try{
        const postCriado = await createPost(postReq);
        res.status(200).json(postCriado);
    }catch(error){
        console.error(error.message);
        res.status(500).json({ error: "Erro ao criar o post" });
    }
}

export const controllerGetPosts = async (req, res) => {
    const posts = await getPosts();
    res.status(200).json(posts);
}

export const controllerGetPostById = async (req, res) => {
    const id = req.params.id;
    const post = await getPostById(id);

    if (!post) {
        // Se não encontrou o post
        return res.status(404).json({ error: "Post não encontrado" });
    }

    // Se encontrou
    return res.status(200).json(post);
}
export const controllerUpdatePost = async (req, res) => {
    const id = req.params.id;
    const dadosAtualizados = req.body;

    if (!ObjectId.isValid(id)) {
        return res.status(400).json({ error: "ID inválido" });
    }

    try {
        const resultado = await updatePost(id, dadosAtualizados);

        if (resultado.matchedCount === 0) {
            return res.status(404).json({ error: "Post não encontrado" });
        }

        return res.status(200).json({ message: "Post atualizado com sucesso" });
    } catch (error) {
        console.error(error.message);
        res.status(500).json({ error: "Erro ao atualizar o post" });
    }
}

export const controllerDeletePost = async (req, res) => {
    const id = req.params.id;

    if (!ObjectId.isValid(id)) {
        return res.status(400).json({ error: "ID inválido" });
    }

    try {
        const resultado = await deletePost(id);

        if (resultado.deletedCount === 0) {
            return res.status(404).json({ error: "Post não encontrado" });
        }

        return res.status(200).json({ message: "Post deletado com sucesso" });
    } catch (error) {
        console.error(error.message);
        res.status(500).json({ error: "Erro ao deletar o post" });
    }
}

export const controllerUploadImage = async (req, res) => {
   const _id = new ObjectId();
   const imgUrl = `http://localhost:3000/${_id}.png`;
   const descricao = req.body.descricao;
   const post = {
     "_id": _id,
     "descricao": descricao,
     "imgUrl": imgUrl
   }
   try{
    const postCriado = await createPost(post);
    const enderecoImg = `uploads/${_id}.png`;
    fs.renameSync(req.file.path, enderecoImg);
    res.status(200).json(postCriado);
   }catch(error){

   }
}