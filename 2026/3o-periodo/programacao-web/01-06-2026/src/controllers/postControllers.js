import { getPosts, getPostById, createPost } from "../models/postModels.js";
import fs from "fs";
import { ObjectId } from "mongodb";

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


export const controllerPostPost = async (req, res) => {
  const postReq = req.body;
  try {
   const postCriado = await createPost(postReq);
   res.status(200).json(postCriado);  
   //postCriado = representação do objeto no 
   //banco, precisamos do id que o mongodb gerou posteriormente
  } catch(erro) {
   console.error(erro.message);
   res.status(500).json({"Erro":"Falha na requisição"})
  }
}

//diferentemente do método controllerPostPost que recebe 
//os dados integralmente no body da req HTTP, aqui a imagem (parte dos dados)
//vem em um formuláio, então não basta simplesmente fazer req.body
//vamos ter que montar um post manualmente 


export const controllerUploadImage = async (req, res) => {
 const _id = new ObjectId();
 const imgUrl = `http://localhost:3000/${_id}.png`;
 const descricao = req.body.descricao; 
 const post = {
  "_id":_id,
  "descricao": descricao, 
  "imgUrl":imgUrl
 };
 try {
    const postCriado = await createPost(post);
    const enderecoImg = `uploads/${_id}.png`;
    fs.renameSync(req.file.path, enderecoImg);
    res.status(200).json(postCriado);
  }catch (erro){
    console.log(erro.message);
    res.status(500).json({"erro":"Falha na requisição..."});
  }
}

