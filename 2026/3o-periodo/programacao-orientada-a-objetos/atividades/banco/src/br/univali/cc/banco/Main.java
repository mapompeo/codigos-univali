package br.univali.cc.banco;

public class Main {
    public static void main(String[] args) {

        Banco banco = new Banco("Banco Univali", 1);

        System.out.println("=== criação de contas ===");
        banco.criarConta(500.00);
        banco.criarConta(1000.00, 500.00);
        banco.criarConta(200.00);

        System.out.println("\n=== depósitos ===");
        banco.depositar(1001, 300.00);
        banco.depositar(1002, 150.00);

        System.out.println("\n=== saques ===");
        banco.sacar(1001, 200.00);
        banco.sacar(1003, 500.00);
        banco.sacar(1002, 1600.00);

        System.out.println("\n=== transferência ===");
        banco.transferir(1001, 1003, 100.00);
        banco.transferir(1003, 1001, 500.00);

        System.out.println("\n=== saldos ===");
        banco.emitirSaldo(1001);
        banco.emitirSaldo(1002);
        banco.emitirSaldo(1003);

        System.out.println("\n=== extratos ===");
        System.out.println(banco.emitirExtrato(1001));
        System.out.println(banco.emitirExtrato(1002));
        System.out.println(banco.emitirExtrato(1003));

        System.out.println("=== exclusão de conta ===");
        banco.excluirConta(1003);
        banco.emitirSaldo(1003);
    }
}
