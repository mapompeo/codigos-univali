package br.univali.empresa;

public class Main {
    public static void main(String[] args) {
        Empresa empresa = new Empresa();

        empresa.adicionarDespesa(new Horista("Carlos", 25, "TI", 160, 25.0f));
        empresa.adicionarDespesa(new Assalariado("Ana", 30, "RH", 4500.0f));
        empresa.adicionarDespesa(new Comissionado("Pedro", 28, "Vendas", 2000.0f, 800.0f));

        empresa.adicionarDespesa(new Fatura(350.0f));
        empresa.adicionarDespesa(new Fatura(120.0f)); 

        empresa.adicionarDespesa(new PrestadorServico(1500.0f));

        System.out.println("Total de gastos: R$ " + empresa.calcularTotalDeGastos());
    }
}
