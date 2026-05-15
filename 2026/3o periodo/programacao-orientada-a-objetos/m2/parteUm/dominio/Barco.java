package parteUm.dominio;

public class Barco {
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
}