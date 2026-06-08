
const addFormEvent = () => {
    const form = document.getElementById("form-post");
    if (form) {
        form.addEventListener("submit", async (e) => {
            e.preventDefault();

        })
    }
}
document.addEventListener("DOMContentLoaded", addFormEvent)