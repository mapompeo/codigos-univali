import { upload } from "./fatchApi.js";

const addFormEvent = () => {
    const form = document.getElementById("form-post");
    if (form) {
        form.addEventListener("submit", async (e) => {
            e.preventDefault();

            const descricaoInput = document.getElementById("descricao");
            const imagemInput = document.getElementById("imagem");

            try {
                const data = await upload(new FormData(form));
                if (data) {
                    console.log("Upload bem-sucedido:", data);
                    window.location.href = "./index.html";
                }
            } catch (error) {
                console.error("Erro no upload:", error);
                alert("Ocorreu um erro ao enviar o post. Por favor, tente novamente.");
            }
        })
    }
}
document.addEventListener("DOMContentLoaded", addFormEvent)