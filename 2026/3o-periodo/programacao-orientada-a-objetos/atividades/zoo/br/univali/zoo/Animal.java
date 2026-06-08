public abstract class Animal {
    private String nome;
    private int idade;

    public Animal(int idade, String nome) {
        this.nome = nome;
        this.idade = idade;
    }
    
    public int getIdade() {
        return idade;
    }
    
    public void setIdade(int idade) {
        this.idade = idade;
    }

    abstract public String emitirSom();
}