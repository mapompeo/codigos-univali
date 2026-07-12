import {controllerGetPosts} from "../controllers/postControllers.js";
import {controllerGetPostById} from "../controllers/postControllers.js";
import {controllerPostPost} from "../controllers/postControllers.js";
import {controllerUploadImage} from "../controllers/postControllers.js";
import {controllerUpdatePost} from "../controllers/postControllers.js";
import {controllerDeletePost} from "../controllers/postControllers.js";
import multer from "multer";

// somente para Windos
const storage = multer.diskStorage({
  destination: function (req, file, cb) {
    // Especifica o diretório para armazenar as imagens enviadas
    cb(null, 'uploads/'); // Substitua por seu caminho de upload desejado
  },
  filename: function (req, file, cb) {
    // Mantém o nome original do arquivo por simplicidade
    cb(null, file.originalname); // Considere usar uma estratégia de geração de nomes únicos para produção
  }
});

const upload = multer({dest: "./uploads", storage});

const getPostRoutes = (app) => {
  app.get("/posts", controllerGetPosts);
  app.get("/posts/:id", controllerGetPostById);
  app.post("/post", controllerPostPost);
  app.post("/upload",upload.single("imagem"), controllerUploadImage);
  app.put("/post/:id", controllerUpdatePost);
  app.delete("/post/:id", controllerDeletePost);
}
export default getPostRoutes;