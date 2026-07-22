# 🚀 Guia Rápido de Teste - MeuPrinter

## 1️⃣ Abra o OrcaSlicer Compilado

```bash
# Opção A: Abrir via Finder (GUI)
open /Users/tonymichaelbatistadelima/Downloads/OrcaSlicer-main/build/arm64/src/Cromalors.app

# Opção B: Abrir via Terminal (pode mostrar logs de debug)
/Users/tonymichaelbatistadelima/Downloads/OrcaSlicer-main/build/arm64/src/Cromalors.app/Contents/MacOS/Cromalors
```

## 2️⃣ Localizar MeuPrinter na Interface

```
Menu: OrcaSlicer → Preferences (ou similar)
  ↓
Section: "Printers" ou "Impressoras"
  ↓
Button: "+ Add Printer" ou "+ Adicionar Impressora"
  ↓
Search/Select: "MeuPrinter"
  ↓
Confirm: OK
```

## 3️⃣ Verificar Configurações Disponíveis

Após adicionar MeuPrinter, você deve ver:

### Nozzles (Bocais)
- ✅ MeuPrinter 0.2 nozzle
- ✅ MeuPrinter 0.4 nozzle (padrão)
- ✅ MeuPrinter 0.6 nozzle
- ✅ MeuPrinter 0.8 nozzle

### Filamentos
- ✅ Generic PLA @MeuPrinter
- ✅ Generic PETG @MeuPrinter
- ✅ Generic ABS @MeuPrinter

### Processos
- ✅ 0.12mm Fine @MeuPrinter
- ✅ 0.20mm Standard @MeuPrinter
- ✅ 0.28mm Draft @MeuPrinter

## 4️⃣ Teste de Fatiamento (Slicing)

```
1. File → Open → Selecionar arquivo .stl ou .3mf
2. Printer: MeuPrinter
3. Nozzle: 0.4mm (padrão)
4. Filament: Generic PLA @MeuPrinter
5. Process: 0.20mm Standard @MeuPrinter
6. Click: "Slice now!" ou "Fatiar"
7. ✅ Verifique se gera código G sem erros
```

## 5️⃣ Verificar Arquivos de Configuração

Se quiser verificar os arquivos diretamente:

```bash
# Mostrar todos os arquivos MeuPrinter criados
find /Users/tonymichaelbatistadelima/Downloads/OrcaSlicer-main/resources/profiles/Custom -name "*MeuPrinter*" -o -name "*meu_printer*"

# Validar sintaxe JSON de todos os arquivos
for f in /Users/tonymichaelbatistadelima/Downloads/OrcaSlicer-main/resources/profiles/Custom/{machine,filament,process}/*MeuPrinter*.json; do
  python3 -m json.tool "$f" > /dev/null && echo "✅ $(basename $f)" || echo "❌ $(basename $f)"
done

# Ver registro do MeuPrinter
grep "MeuPrinter" /Users/tonymichaelbatistadelima/Downloads/OrcaSlicer-main/resources/profiles/Custom.json | head -10
```

## 6️⃣ Se algo não funcionar

### MeuPrinter não aparece
```bash
# Verifique se os arquivos estão presentes
ls /Users/tonymichaelbatistadelima/Downloads/OrcaSlicer-main/resources/profiles/Custom/machine/MeuPrinter.json

# Verifique se Custom.json tem referências
grep -c "MeuPrinter" /Users/tonymichaelbatistadelima/Downloads/OrcaSlicer-main/resources/profiles/Custom.json

# Limpe cache do OrcaSlicer e tente novamente
rm -rf ~/.OrcaSlicer* 2>/dev/null
# Re-abra o OrcaSlicer
```

### Erro ao fatiar
```bash
# Verifique os logs
cat ~/.OrcaSlicer*/OrcaSlicer.log | tail -50

# Valide o arquivo STL/3MF
file seu_arquivo.stl
```

## 📊 Resultado Esperado

Se tudo funcionar corretamente:

```
✅ MeuPrinter aparece na lista de impressoras
✅ 4 nozzles disponíveis para seleção
✅ 3 filamentos compatíveis com MeuPrinter
✅ 3 processos de qualidade disponíveis
✅ Fatiamento sem erros
✅ Código G gerado corretamente
✅ Color Mixing GUI pronto (após integração manual)
```

## 🎓 Próximas Ações

1. **Teste básico** - Abra OrcaSlicer e verifique MeuPrinter
2. **Teste funcional** - Fatiar um modelo STL
3. **Customização** - Ajuste temperaturas/velocidades conforme necessário
4. **Integração GUI** - Siga `ColorMixing_Integration.md` para adicionar aba de mistura de cores

---

**Status:** ✅ PRONTO PARA USAR!

Para suporte, consulte:
- 📄 `MEUPRINTER_FINAL_REPORT.md` - Relatório completo
- 📄 `ColorMixing_Integration.md` - Integração da GUI
- 📄 `README_MeuPrinter_PT.md` - Documentação em português

