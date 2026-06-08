package parteDois.dominio;

public class Barco implements Veiculo {
    private String nome;
    private String capitao;

    public Barco(String nome) {
        this.nome = nome;
    }

    public void informarCapitao(String nome) {
        this.capitao = nome;
    }

    public String getNome() {
        return nome;
    }

    @Override
    public String obterDescricao() {
        return "Nome: " + nome + ", Capitão: " + capitao;
    }
}