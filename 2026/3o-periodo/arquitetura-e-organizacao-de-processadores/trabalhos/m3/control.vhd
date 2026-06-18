-- =============================================================================
--  control.vhd  --  Unidade de Controle  --  RISC-V 32-bit Monociclo
-- =============================================================================
--
--  Entradas:  opcode (7 bits), funct3 (3 bits), funct7b5 (1 bit)
--  Saídas (sinais de controle):
--
--    reg_write  : habilita escrita no banco de registradores
--    alu_src    : 0 = usa rs2,  1 = usa imediato
--    mem_read   : habilita leitura da memória de dados
--    mem_write  : habilita escrita na memória de dados
--    mem_to_reg : 0 = resultado da ULA → rd,  1 = dado da mem → rd
--    branch     : instrução é um branch
--    jump       : instrução é JAL ou JALR
--    lui        : instrução é LUI  (rd = imm)
--    auipc      : instrução é AUIPC (rd = PC + imm)
--    jalr       : instrução é JALR (modifica cálculo do target)
--    alu_op     : código de operação para a ULA
-- =============================================================================

library ieee;
use ieee.std_logic_1164.all;

entity Control is
    port (
        opcode    : in  std_logic_vector(6 downto 0);
        funct3    : in  std_logic_vector(2 downto 0);
        funct7b5  : in  std_logic;

        reg_write : out std_logic;
        alu_src   : out std_logic;
        mem_read  : out std_logic;
        mem_write : out std_logic;
        mem_to_reg: out std_logic;
        branch    : out std_logic;
        jump      : out std_logic;
        lui       : out std_logic;
        auipc     : out std_logic;
        jalr      : out std_logic;
        alu_op    : out std_logic_vector(3 downto 0)
    );
end Control;

architecture rtl of Control is
begin
    process(opcode, funct3, funct7b5)
    begin
        -- defaults: todos desligados
        reg_write <= '0'; alu_src   <= '0'; mem_read  <= '0';
        mem_write <= '0'; mem_to_reg<= '0'; branch    <= '0';
        jump      <= '0'; lui       <= '0'; auipc     <= '0';
        jalr      <= '0'; alu_op    <= "0000";

        case opcode is

            -- ----------------------------------------------------------------
            --  R-type  (add sub and or xor sll srl sra slt sltu)
            -- ----------------------------------------------------------------
            when "0110011" =>
                reg_write <= '1';
                case funct3 is
                    when "000" =>
                        if funct7b5 = '1' then alu_op <= "0001"; -- SUB
                        else                   alu_op <= "0000"; end if; -- ADD
                    when "111" => alu_op <= "0010"; -- AND
                    when "110" => alu_op <= "0011"; -- OR
                    when "100" => alu_op <= "0100"; -- XOR
                    when "001" => alu_op <= "0101"; -- SLL
                    when "101" =>
                        if funct7b5 = '1' then alu_op <= "0111"; -- SRA
                        else                   alu_op <= "0110"; end if; -- SRL
                    when "010" => alu_op <= "1000"; -- SLT
                    when "011" => alu_op <= "1001"; -- SLTU
                    when others => null;
                end case;

            -- ----------------------------------------------------------------
            --  I-type aritmética (addi slti sltiu andi ori xori slli srli srai)
            -- ----------------------------------------------------------------
            when "0010011" =>
                reg_write <= '1'; alu_src <= '1';
                case funct3 is
                    when "000" => alu_op <= "0000"; -- ADDI
                    when "010" => alu_op <= "1000"; -- SLTI
                    when "011" => alu_op <= "1001"; -- SLTIU
                    when "111" => alu_op <= "0010"; -- ANDI
                    when "110" => alu_op <= "0011"; -- ORI
                    when "100" => alu_op <= "0100"; -- XORI
                    when "001" => alu_op <= "0101"; -- SLLI
                    when "101" =>
                        if funct7b5 = '1' then alu_op <= "0111"; -- SRAI
                        else                   alu_op <= "0110"; end if; -- SRLI
                    when others => null;
                end case;

            -- ----------------------------------------------------------------
            --  Load  (lw — funct3=010)
            -- ----------------------------------------------------------------
            when "0000011" =>
                reg_write  <= '1'; alu_src    <= '1';
                mem_read   <= '1'; mem_to_reg <= '1';
                alu_op <= "0000"; -- ADD (calcula endereço)

            -- ----------------------------------------------------------------
            --  Store  (sw — funct3=010)
            -- ----------------------------------------------------------------
            when "0100011" =>
                alu_src   <= '1'; mem_write <= '1';
                alu_op <= "0000"; -- ADD (calcula endereço)

            -- ----------------------------------------------------------------
            --  Branch  (beq bne blt bge bltu bgeu)
            -- ----------------------------------------------------------------
            when "1100011" =>
                branch <= '1';
                case funct3 is
                    when "000" => alu_op <= "0001"; -- BEQ  (SUB, testa zero)
                    when "001" => alu_op <= "0001"; -- BNE  (SUB, testa ~zero)
                    when "100" => alu_op <= "1000"; -- BLT  (SLT)
                    when "101" => alu_op <= "1000"; -- BGE  (SLT invertido)
                    when "110" => alu_op <= "1001"; -- BLTU
                    when "111" => alu_op <= "1001"; -- BGEU
                    when others => null;
                end case;

            -- ----------------------------------------------------------------
            --  LUI
            -- ----------------------------------------------------------------
            when "0110111" =>
                reg_write <= '1'; lui <= '1';

            -- ----------------------------------------------------------------
            --  AUIPC
            -- ----------------------------------------------------------------
            when "0010111" =>
                reg_write <= '1'; auipc <= '1'; alu_op <= "0000";

            -- ----------------------------------------------------------------
            --  JAL
            -- ----------------------------------------------------------------
            when "1101111" =>
                reg_write <= '1'; jump <= '1';

            -- ----------------------------------------------------------------
            --  JALR
            -- ----------------------------------------------------------------
            when "1100111" =>
                reg_write <= '1'; jump <= '1'; jalr <= '1';
                alu_src <= '1';   alu_op <= "0000";

            when others => null;
        end case;
    end process;
end rtl;
