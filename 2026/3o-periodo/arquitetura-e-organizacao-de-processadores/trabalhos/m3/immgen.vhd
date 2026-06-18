-- =============================================================================
--  immgen.vhd  --  Gerador de Imediato  --  RISC-V 32-bit Monociclo
-- =============================================================================
--
--  Extrai e estende com sinal o campo imediato de qualquer formato RISC-V:
--
--  Formato  Opcode          Uso
--  -------  --------------  -----------------------------------------
--  I-type   0010011         ADDI, ANDI, ORI, XORI, SLTI, shifts imm
--           0000011         LW (Load)
--           1100111         JALR
--  S-type   0100011         SW (Store)
--  B-type   1100011         BEQ, BNE, BLT, BGE, ...
--  U-type   0110111 / 0010111  LUI / AUIPC
--  J-type   1101111         JAL
-- =============================================================================

library ieee;
use ieee.std_logic_1164.all;

entity ImmGen is
    port (
        instr : in  std_logic_vector(31 downto 0);
        imm   : out std_logic_vector(31 downto 0)
    );
end ImmGen;

architecture rtl of ImmGen is
begin
    process(instr)
        variable op : std_logic_vector(6 downto 0);
    begin
        op := instr(6 downto 0);
        case op is

            -- I-type: sign_ext( instr[31:20] )
            when "0010011" | "0000011" | "1100111" =>
                imm <= (31 downto 12 => instr(31)) & instr(31 downto 20);

            -- S-type: sign_ext( instr[31:25] & instr[11:7] )
            when "0100011" =>
                imm <= (31 downto 12 => instr(31)) &
                       instr(31 downto 25) & instr(11 downto 7);

            -- B-type: sign_ext( {instr[31], instr[7], instr[30:25], instr[11:8], 0} )
            when "1100011" =>
                imm <= (31 downto 13 => instr(31)) &
                       instr(31) & instr(7) &
                       instr(30 downto 25) & instr(11 downto 8) & '0';

            -- U-type: instr[31:12] & 12'b0
            when "0110111" | "0010111" =>
                imm <= instr(31 downto 12) & (11 downto 0 => '0');

            -- J-type: sign_ext( {instr[31], instr[19:12], instr[20], instr[30:21], 0} )
            when "1101111" =>
                imm <= (31 downto 21 => instr(31)) &
                       instr(31) & instr(19 downto 12) &
                       instr(20) & instr(30 downto 21) & '0';

            when others =>
                imm <= (others => '0');
        end case;
    end process;
end rtl;
