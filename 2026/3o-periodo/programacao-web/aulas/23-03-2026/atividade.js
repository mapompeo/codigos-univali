const prompt = require('prompt-sync')()
let tipoCombustivel = prompt("Digite o tipo de combustível: ")
let precoGasolina = 7.01
let precoEtanol = 5.79
let precoDiesel = 4.59

let kmPercorridos = prompt("Quantos km percorreu?")
let kmPorLitro = prompt("Quantos km por litro?")
let litrosConsumidos = kmPercorridos / kmPorLitro
let custoViagem = 0;

if (tipoCombustivel === 'etanol') {
    custoViagem = (litrosConsumidos * precoEtanol).toFixed(2)
}
else if (tipoCombustivel === 'gasolina') {
    custoViagem = (litrosConsumidos * precoGasolina).toFixed(2)
}
else if (tipoCombustivel === 'diesel') {
    custoViagem = (litrosConsumidos * precoDiesel).toFixed(2)
}

let mensagem = `O custo é de R${custoViagem}`
console.log(mensagem)
