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
--  ula completa
--  mux da fonte da ula
--  gerador de imediatos (immgen)
--  unidade de controle (control)
--  program counter (pc)
--  memória de instruções (instrfile)
--  lógica de branch e jump
--  mux de writeback (ula / imm / pc+imm / pc+4)
-- =============================================================================

library IEEE;
use IEEE.std_logic_1164.all;

entity design is
    port(
        i_clk : in  std_logic;
        i_rst : in  std_logic;
        o_pc  : out std_logic_vector(31 downto 0);  -- para debug
        o_ula : out std_logic_vector(31 downto 0);  -- para debug
        o_zero: out std_logic                        -- para debug
    );
end design;


architecture arch of design is

    -- PC e instrução
    signal w_pc        : std_logic_vector(31 downto 0);
    signal w_pc_next   : std_logic_vector(31 downto 0);
    signal w_pc_plus4  : std_logic_vector(31 downto 0);
    signal w_pc_imm    : std_logic_vector(31 downto 0); -- PC + imm (branch / JAL / AUIPC)
    signal w_inst      : std_logic_vector(31 downto 0);

    -- Datapath
    signal w_imm         : std_logic_vector(31 downto 0);
    signal w_rs1_data    : std_logic_vector(31 downto 0);
    signal w_rs2_data    : std_logic_vector(31 downto 0);
    signal w_mux_alu_out : std_logic_vector(31 downto 0);
    signal w_ula         : std_logic_vector(31 downto 0);
    signal w_zero        : std_logic;
    signal w_dmem_rdata  : std_logic_vector(31 downto 0); -- dado lido da memória de dados
    signal w_wdata       : std_logic_vector(31 downto 0); -- dado escrito no registrador

    -- Sinais de controle
    signal reg_write  : std_logic;
    signal alu_src    : std_logic;
    signal mem_read   : std_logic;
    signal mem_write  : std_logic;
    signal mem_to_reg : std_logic;
    signal branch     : std_logic;
    signal jump       : std_logic;
    signal lui        : std_logic;
    signal auipc      : std_logic;
    signal jalr       : std_logic;
    signal alu_op     : std_logic_vector(3 downto 0);

    -- Lógica de desvio
    signal branch_taken : std_logic;

begin

    -- =========================================================================
    -- CAMINHO DO PC
    -- =========================================================================

    u_PC: entity work.PC
    port map(i_clk, i_rst, w_pc_next, w_pc);

    -- PC + 4 (próxima instrução sequencial)
    u_PLUS4: entity work.adder
    port map(w_pc, x"00000004", w_pc_plus4);

    -- PC + imm (alvo de branch, JAL e AUIPC)
    u_PC_IMM: entity work.adder
    port map(w_pc, w_imm, w_pc_imm);

    -- Memória de instruções
    u_INSTRFILE: entity work.InstrFile
    port map(w_pc, w_inst);

    -- =========================================================================
    -- LÓGICA DO PRÓXIMO PC
    -- =========================================================================

    -- Condição de desvio baseada em funct3 e saída da ULA
    process(branch, w_inst, w_zero, w_ula)
    begin
        branch_taken <= '0';
        if branch = '1' then
            case w_inst(14 downto 12) is
                when "000" => branch_taken <= w_zero;           -- BEQ: desvia se zero
                when "001" => branch_taken <= not w_zero;       -- BNE: desvia se não zero
                when "100" => branch_taken <= w_ula(0);         -- BLT: desvia se a < b (SLT)
                when "101" => branch_taken <= not w_ula(0);     -- BGE: desvia se a >= b
                when "110" => branch_taken <= w_ula(0);         -- BLTU
                when "111" => branch_taken <= not w_ula(0);     -- BGEU
                when others => null;
            end case;
        end if;
    end process;

    -- Mux do próximo PC
    process(jalr, jump, branch_taken, w_pc_plus4, w_pc_imm, w_ula)
    begin
        if jalr = '1' then
            -- JALR: (rs1 + imm), bit 0 forçado a 0
            w_pc_next <= w_ula(31 downto 1) & '0';
        elsif jump = '1' or branch_taken = '1' then
            -- JAL ou branch tomado: PC + imm
            w_pc_next <= w_pc_imm;
        else
            -- Sequencial: PC + 4
            w_pc_next <= w_pc_plus4;
        end if;
    end process;

    -- =========================================================================
    -- DATAPATH
    -- =========================================================================

    u_IMMGEN: entity work.ImmGen
    port map(w_inst, w_imm);

    u_CONTROL: entity work.Control
    port map(
        w_inst(6 downto 0),
        w_inst(14 downto 12),
        w_inst(30),   -- funct7b5 = bit 30 da instrução (bit 5 do campo funct7)
        reg_write,
        alu_src,
        mem_read,
        mem_write,
        mem_to_reg,
        branch,
        jump,
        lui,
        auipc,
        jalr,
        alu_op
    );

    u_REGBANK: entity work.RegFile
    port map(
        i_clk,
        reg_write,
        w_inst(19 downto 15),  -- rs1
        w_inst(24 downto 20),  -- rs2
        w_inst(11 downto  7),  -- rd
        w_wdata,               -- dado a escrever (vem do mux de writeback)
        w_rs1_data,
        w_rs2_data
    );

    -- Mux da fonte da ULA: rs2 ou imediato
    u_MUX_ALU: entity work.mux21
    port map(w_rs2_data, w_imm, alu_src, w_mux_alu_out);

    u_ULA: entity work.ula
    port map(w_rs1_data, w_mux_alu_out, alu_op, w_ula, w_zero);

    -- Memória de dados (LW / SW)
    -- addr = resultado da ULA (rs1 + imm), wdata = rs2
    u_DMEM: entity work.DataMem
    port map(i_clk, mem_write, w_ula, w_rs2_data, w_dmem_rdata);

    -- =========================================================================
    -- MUX DE WRITEBACK: seleciona o dado escrito no banco de registradores
    --   LUI       → imediato direto
    --   AUIPC     → PC + imediato
    --   JAL/JALR  → PC + 4 (endereço de retorno)
    --   LW        → dado lido da memória de dados
    --   default   → resultado da ULA
    -- =========================================================================
    process(lui, auipc, jump, mem_to_reg, w_ula, w_imm, w_pc_imm, w_pc_plus4, w_dmem_rdata)
    begin
        if lui = '1' then
            w_wdata <= w_imm;
        elsif auipc = '1' then
            w_wdata <= w_pc_imm;
        elsif jump = '1' then
            w_wdata <= w_pc_plus4;
        elsif mem_to_reg = '1' then
            w_wdata <= w_dmem_rdata;
        else
            w_wdata <= w_ula;
        end if;
    end process;

    -- =========================================================================
    -- SAÍDAS PARA DEBUG
    -- =========================================================================
    o_pc   <= w_pc;
    o_ula  <= w_ula;
    o_zero <= w_zero;

end arch;
