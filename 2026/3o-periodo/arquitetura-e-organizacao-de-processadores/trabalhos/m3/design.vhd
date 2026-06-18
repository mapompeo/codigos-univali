-- =============================================================================
--  design.vhd  --  Arquivo topo  --  RISC-V 32-bit Monociclo
-- =============================================================================
--
--  Entradas (op):
--    a (será substituido no futuro pela saída a do regfile)
--    b (será substituido no futuro pela saída b do regfile)
--    imm (será substituído futuramenteimediato pelo imm gerado a partir da instrução)
--    op (seletor da operação da ula)
--    alusrc (seleciona entre a entrada b e a entrada imm)
--
--  Saídas:
--    o_ula  : resultado de 32 bits
--    o_zero : '1' quando res = 0  (usado por BEQ/BNE)
--
--  Testes:
--    faça testes para validar cada uma das operações da ula
--    faça testes para verificar se o alusrc escolhe entre b ou imediato
-- =============================================================================
--library IEEE;
--use IEEE.std_logic_1164.all;
--
--entity design is
--    port(i_a,i_b, i_imm : in std_logic_vector(31 downto 0);
--         i_ula_src : in std_logic;
--         i_ula_op : in std_logic_vector(3 downto 0);
--         o_ula : out std_logic_vector(31 downto 0);
--         o_zero : out std_logic
--         );
--end design;
--architecture arch of design is
--    signal w_mux_b, w_ula_b : std_logic_vector(31 downto 0);
--begin
--    -- chamada da função do código chamado ula
--    u_ULA : entity work.ula
--    port map(i_a, w_ula_b, i_ula_op, o_ula, o_zero);
--
--    -- mux que escolhe entre o rs2 e o imediato para colocar na entrada b da ula
--    u_MUX_ULA_FONTE: entity work.mux21
--    port map(i_b, i_imm, i_ula_src, w_ula_b);
--
--end arch;
-- =============================================================================


-- =============================================================================
-- PROJETO FUNCIONANDO COM
--  banco de registradores (regfile)
--  ula incompleta
--  mux da fonte da ual
--
-- PROXIMO PASSO
-- incluir o cálculo do pc no projeto
-- códigos necessários fornecidos
--   intrfile
--   pc
--   immgen
--
-- NECESSÁRIO CRIAR
--   caminho que calcula pc+4 e realimenta o pc com esse valor
--
-- TESTE
-- testar se o código roda as intruções que estão no intrfile
--
-- no testbench será necessário apenas simular períodos e os sinais do controle que ainda não fizemos
-- pois as instruções serão lidas para coletar entradas de rs1, rs2, rd
-- =============================================================================

library IEEE;
use IEEE.std_logic_1164.all;

entity design is
    port(
            i_clk : in std_logic;
            i_inst: in  std_logic_vector(31 downto 0);

            i_ula_op: in  std_logic_vector( 3 downto 0);
            i_sel   : in  std_logic; --seleciona entre b e imm
            o_ula   : out std_logic_vector(31 downto 0);
            o_zero  : out std_logic
        );
end design;


architecture arch of design is
    signal w_mux_alu_out : std_logic_vector(31 downto 0);

    signal w_rs1_data, w_rs2_data, w_ula : std_logic_vector(31 downto 0);

    signal w_imm    : std_logic_vector(31 downto 0);

    -- FIOS DE CONTROLE
    signal w_wr_ena    : std_logic;

begin
    u_ULA : entity work.ula
    port map(w_rs1_data, w_mux_alu_out, i_ula_op, w_ula, o_zero);

    u_MUX : entity work.mux21
    port map(w_rs2_data, w_imm, i_sel, w_mux_alu_out);

    u_REGBANK: entity work.RegFile
    port map( i_clk, w_wr_ena, i_inst(19 downto 15), i_inst(24 downto 20),
              i_inst(11 downto 7), w_ula, w_rs1_data, w_rs2_data);

   u_IMMGEN: entity work.ImmGen
    port map(i_inst , w_imm);

   u_CONTROL: entity Control
    port map(
        i_inst(6 downto 0),
        i_inst(14 downto 12),
        i_inst(31 downto 25), -- aqui é apenas um bit fizemos errado em sala.

        reg_write ,
        alu_src   ,
        mem_read  ,
        mem_write ,
        mem_to_reg,
        branch    ,
        jump      ,
        lui       ,
        auipc     ,
        jalr      ,
        alu_op
    );


    --SINAIS EXPOSTOS PARA TESTE
    o_ula <= w_ula;

end arch;
