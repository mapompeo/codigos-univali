import express from "express";
import formData from "express-form-data";
import 'dotenv/config';
import getPostRoutes from "./src/routes/postRoutes.js"

const corsOptions = {
    origin: "*",
    methods: "GET,HEAD,PUT,PATCH,POST,DELETE",
    optionsSuccessStatus: 200
};

const app = express();
app.use(express.json());
app.use(formData.format());
app.use(express.static("uploads"));
getPostRoutes(app);
app.listen(3000, () => {
    console.log("Servidor escutando...");
});