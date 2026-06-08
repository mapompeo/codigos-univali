const API_URL = "http://127.0.0.1:3000";

export const getPostApi = async () => {
    try {
        const resposta = await fetch(`${API_URL}/posts`);
        return await resposta.json();
    } catch (error) {
        console.error("Erro ao buscar os posts:", error);
        return [];
    }
}