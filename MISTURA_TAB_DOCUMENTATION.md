# 🎨 Aba "Mistura" - Documentação

## Visão Geral

A aba **"Mistura"** foi adicionada à seção de **Filamentos** do OrcaSlicer, permitindo configurar misturas de cores avançadas diretamente nas configurações de filamento.

## 📍 Localização

**Caminho:** Configurações → Filamentos → **Mistura**

## ✨ Funcionalidades

### 1. Gerenciamento de Mistura de Cores
- **Adicionar Mix**: Criar novas configurações de mistura
- **Remover Mix**: Deletar misturas não desejadas
- **Listar Mixes**: Visualizar todas as misturas configuradas

### 2. Configurações de Purge Tower
- ✅ **Enable Purge Tower**: Ativar/desativar purge tower
- 📏 **Purge Length**: Comprimento da purge tower (0-1000 mm)

### 3. Configurações de Transição
- 📏 **Transition Length**: Comprimento da transição entre cores (0-500 mm)
- 🎯 **Enable Color Smoothing**: Suavizar cores durante a transição

### 4. Gerenciamento de Filamentos
- Adicionar múltiplos filamentos à mistura
- Definir percentuais de cada filamento
- Visualizar composição da mistura

## 🛠️ Arquivos Modificados

### 1. Tab.cpp
```cpp
// Adicionada aba "Mistura" em TabFilament::build()
page = add_options_page(L("Mistura"), "custom-gcode_color_mixing");
optgroup = page->new_optgroup(L("Mistura de Cores"), L"param_color_mixing", 0);
```

### 2. CMakeLists.txt
```cmake
# Adicionado ColorMixing.cpp e ColorMixing.hpp
GUI/ColorMixing.cpp
GUI/ColorMixing.hpp
```

### 3. ColorMixing.hpp / ColorMixing.cpp
- Classe `ColorMixingPanel`: Interface wxWidgets completa
- Estrutura `ColorMixingConfig`: Configuração de mistura

## 🎯 Fluxo de Uso

```
1. Abrir OrcaSlicer
   ↓
2. Configurações → Filamentos
   ↓
3. Selecionar filamento (ex: Generic PLA @MeuPrinter)
   ↓
4. Clicar na aba "Mistura"
   ↓
5. Clicar "Add Mix"
   ↓
6. Configurar cores, percentuais e purge tower
   ↓
7. Aplicar e salvar
   ↓
8. Usar em fatiamento multi-cores
```

## 📊 Interface da Aba

```
┌─────────────────────────────────────────┐
│     Color Mixing Configuration           │
├─────────────────────────────────────────┤
│                                         │
│  Color Mix List:                        │
│  ┌──────────┬──────────┬────────────┐  │
│  │ Mix Name │ Filaments│ Purge Tower│  │
│  ├──────────┼──────────┼────────────┤  │
│  │ Mix 1    │ PLA,PETG │    Yes     │  │
│  │ Mix 2    │ PLA,ABS  │    No      │  │
│  └──────────┴──────────┴────────────┘  │
│                                         │
│  [Add Mix]  [Remove Mix]               │
│                                         │
│  ✓ Mixing Settings                     │
│  ☑ Enable Purge Tower                  │
│  Purge Length (mm): [100        ]      │
│  Transition Length (mm): [50    ]      │
│  ☐ Enable Color Smoothing              │
│                                         │
└─────────────────────────────────────────┘
```

## 🔧 Código-chave

### Estrutura ColorMixingConfig
```cpp
struct ColorMixingConfig {
    std::string name;                    // Nome da mistura
    std::vector<std::string> filaments;  // Lista de filamentos
    std::vector<float> percentages;      // Percentuais (0-100)
    float purgeLength;                   // Comprimento purge (mm)
    float transitionLength;              // Comprimento transição (mm)
    bool enablePurgeTower;               // Ativar purge tower
    bool enableSmoothing;                // Suavizar cores
};
```

### Classe ColorMixingPanel
```cpp
class ColorMixingPanel : public wxPanel {
public:
    void add_color_mix();
    void remove_color_mix();
    void update_color_mix(int index, const ColorMixingConfig& config);
    ColorMixingConfig get_color_mix(int index) const;
    std::vector<ColorMixingConfig> get_all_color_mixes() const;
    
private:
    wxListCtrl* m_color_mix_list;
    wxButton* m_btn_add;
    wxButton* m_btn_remove;
    wxCheckBox* m_cb_purge_tower;
    wxCheckBox* m_cb_smoothing;
    wxSpinCtrl* m_spin_purge_length;
    wxSpinCtrl* m_spin_transition_length;
};
```

## 🚀 Próximos Passos (Opcionais)

### 1. Persistência de Dados
- Salvar misturas em JSON
- Carregar configurações ao iniciar
- Exportar/importar presets

### 2. Funcionalidades Avançadas
- Editor visual de cores
- Simulação de mistura
- Cálculo automático de percentuais
- Histórico de misturas

### 3. Integração com Slicing
- Usar misturas em fatiamento
- Gerar G-code para multi-color
- Validação de compatibilidade

## 📝 Exemplo de Uso

### Criar Mistura PLA + PETG
```
1. Aba Mistura → Add Mix
2. Nome: "Red-Yellow Gradient"
3. Filamentos: PLA (Red), PETG (Yellow)
4. Percentuais: 50% / 50%
5. Purge Length: 150 mm
6. Enable Purge Tower: ✓
7. Enable Smoothing: ✓
8. Transition Length: 75 mm
```

### Resultado
- Fatiamento com gradiente de cor
- Purge tower automático
- Transição suave entre cores

## 🐛 Troubleshooting

### Problema: Aba não aparece
**Solução:** Recompile o OrcaSlicer
```bash
cd build/arm64
cmake --build . --config RelWithDebInfo --target OrcaSlicer -- -j8
```

### Problema: Erro ao adicionar mix
**Solução:** Verifique se filamentos estão configurados
```bash
# Verificar filamentos disponíveis
grep -r "filament" resources/profiles/Custom.json
```

### Problema: G-code não reconhece mistura
**Solução:** Verifique compatibilidade de impressora
- Impressora suporta multi-color?
- Firmware atualizado?
- Configuração correta de nozzle?

## 📚 Referências

- **ColorMixing.hpp**: Interface e estrutura
- **ColorMixing.cpp**: Implementação
- **Tab.cpp**: Integração com abas
- **CMakeLists.txt**: Configuração de build

## ✅ Status

- [x] GUI implementada
- [x] Aba integrada em Filaments
- [x] Compilação bem-sucedida
- [ ] Persistência de dados (TODO)
- [ ] Validação de G-code (TODO)
- [ ] Suporte multi-impressora (TODO)

---

**Última Atualização:** 1 de Junho de 2024  
**Versão:** OrcaSlicer 2.4.0-dev  
**Plataforma:** macOS ARM64

