
const precos = {
    havaianas: 50.00,
    alpargatas: 100.00
};


const atualizarPreco = (produto) => {
    let quantidade = parseInt(document.getElementById(produto + 'Qtd').value) || 0;
    let precoTotal = quantidade * precos[produto];
    document.getElementById(produto + 'Preco').innerText = 'R$ ' + precoTotal.toFixed(2);
}


const salvarProdutos = () => {
    const produtos = {
        havaianas: {
            quantidade: document.getElementById('havaianasQtd').value,
            preco: document.getElementById('havaianasPreco').innerText,
        },
        alpargatas: {
            quantidade: document.getElementById('alpargatasQtd').value,
            preco: document.getElementById('alpargatasPreco').innerText,
        }
    };
    localStorage.setItem('produtos', JSON.stringify(produtos));
}