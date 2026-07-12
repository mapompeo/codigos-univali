import express from "express";
import 'dotenv/config';
import formData from "express-form-data";
import getPostRoutes from "./src/routes/postRoutes.js"

const app = express();
app.use(express.json());
app.use(formData.format());
app.use(express.static("uploads"));
getPostRoutes(app);
app.listen(3000, () => {
    console.log("Servidor escutando...");
});