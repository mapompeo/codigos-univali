const salvarEndereco = () => {
    sessionStorage.setItem('rua', document.getElementById('rua').value);
    sessionStorage.setItem('cidade', document.getElementById('cidade').value);
    sessionStorage.setItem('estado', document.getElementById('estado').value);
    sessionStorage.setItem('cep', document.getElementById('cep').value);
}

const carregarEndereco = () => {
    salvarEndereco();

    let rua = sessionStorage.getItem('rua');
    let cidade = sessionStorage.getItem('cidade');
    let estado = sessionStorage.getItem('estado');
    let cep = sessionStorage.getItem('cep');

    let mensagem = `Rua: ${rua}, cidade: ${cidade}, estado: ${estado}, cep ${cep}`;
    console.log(mensagem);
}