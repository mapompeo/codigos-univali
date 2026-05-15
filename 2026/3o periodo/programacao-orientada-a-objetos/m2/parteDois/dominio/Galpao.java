package parteDois.dominio;

public class Galpao {
    private final Veiculo[] veiculos = new Veiculo[1000];
    private int totalVeiculos = 0;

    public void adicionarVeiculo(Veiculo veiculo) {
        if (totalVeiculos >= veiculos.length) {
            throw new IllegalStateException("Galpão cheio (máx: " + veiculos.length + ")");
        }
        veiculos[totalVeiculos++] = veiculo;
    }
    
    public void imprimirRelacaoVeiculos() {
        for (int i = 0; i < totalVeiculos; i++) {
            System.out.println(veiculos[i].obterDescricao());
        }
    }
}
