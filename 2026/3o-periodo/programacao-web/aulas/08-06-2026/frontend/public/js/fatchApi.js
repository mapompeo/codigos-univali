const API_URL = "http://127.0.0.1:3000";

export const getPostsApi = async () => {
    try {
        const resposta = await fetch(`${API_URL}/posts`);
        return await resposta.json();
    } catch (error) {
        console.error("Erro ao buscar os posts:", error);
        return [];
    }
}

export const upload = async (formData) => {
    try {
        const post = {
            method: "POST",
            body: formData
        }

        const response = await fetch(`${API_URL}/upload`, post);
        if (!response.ok) {
            throw new Error(`Erro na resposta: ${response.statusText}`);
        }

        return await response.json();
    } catch (error) {
        console.error("Erro ao fazer upload:", error);
        return null;
    }
}