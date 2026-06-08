function calcular() {
    let objetivo = parseFloat(document.getElementById("objetivo").value);
    let inicial = parseFloat(document.getElementById("inicial").value);
    let mensal = parseFloat(document.getElementById("mensal").value);
    let juros = parseFloat(document.getElementById("juros").value);
    let divResultado = document.getElementById("resultado");

    if (!objetivo || objetivo <= 0 || !inicial || inicial <= 0 || !mensal || mensal <= 0 || isNaN(juros) || juros < 0) {
        alert("Por favor, preencha todos os campos com valores maiores que zero! (Juros pode ser 0 ou mais)");
        return;
    }

    let saldo = inicial;
    let meses = 0;
    let taxaJuros = juros / 100;

    if (saldo >= objetivo) {
        divResultado.innerHTML = "Voce ja alcancou seu objetivo!";
        return;
    }

    while (saldo < objetivo && meses < 1000) {
        saldo = saldo + (saldo * taxaJuros);
        saldo = saldo + mensal;
        meses++;
    }

    if (meses >= 1000) {
        divResultado.innerHTML = "Com esses valores, vai demorar mais de 1000 meses!";
    } else {
        divResultado.innerHTML = "Voce vai alcancar seu objetivo em " + meses + " meses!";
    }
}

function limpar() {
    document.getElementById("objetivo").value = "";
    document.getElementById("inicial").value = "";
    document.getElementById("mensal").value = "";
    document.getElementById("juros").value = "";
    document.getElementById("resultado").innerHTML = "";
}