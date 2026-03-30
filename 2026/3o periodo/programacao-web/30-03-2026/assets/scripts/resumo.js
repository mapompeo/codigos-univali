const carregarEndereco = function () {
    let endereco = JSON.parse(sessionStorage.getItem('endereco'))

    document.getElementById('rua').innerText = endereco.rua
    document.getElementById('cidade').innerText = endereco.cidade
    document.getElementById('estado').innerText = endereco.estado
    document.getElementById('cep').innerText = endereco.cep

    console.log(endereco)
}

const carregarProdutos = function () {
    let produtos = JSON.parse(sessionStorage.getItem('produtos'))

    document.getElementById('hav_quantidade').innerText = produtos.havaianas.quantidade
    document.getElementById('hav_preco').innerText = produtos.havaianas.preco
    document.getElementById('alp_quantidade').innerText = produtos.alpargatas.quantidade
    document.getElementById('alp_preco').innerText = produtos.alpargatas.preco
    console.log(produtos)
}


carregarEndereco()
carregarProdutos()