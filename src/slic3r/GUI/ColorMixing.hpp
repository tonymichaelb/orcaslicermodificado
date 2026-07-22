#ifndef slic3r_GUI_ColorMixing_hpp_
#define slic3r_GUI_ColorMixing_hpp_

#include <wx/wx.h>
#include <wx/clrpicker.h>
#include <wx/spinctrl.h>
#include <functional>
#include <array>
#include <string>
#include <vector>

namespace Slic3r {
namespace GUI {

// Structure to hold color mixing configuration
struct ColorMixingConfig {
    std::string name;
    std::vector<std::string> filaments;  // Selected filaments for mixing
    std::vector<float> percentages;       // Percentage of each filament (0-100)
    float purgeLength { 100.f };          // Purge tower length in mm
    float transitionLength { 50.f };      // Transition length between colors
    bool enablePurgeTower { true };       // Enable/disable purge tower
    bool enableSmoothing { false };       // Enable color smoothing
};

class ColorMixingPanel : public wxPanel {
    wxDECLARE_EVENT_TABLE();

public:
    using ApplyFn = std::function<void(int, int, int, const wxColour&, const std::vector<int>&, double)>;

    ColorMixingPanel(wxWindow* parent, wxWindowID id = wxID_ANY, ApplyFn apply_fn = nullptr);
    ~ColorMixingPanel() = default;

    void add_color_mix();
    void remove_color_mix();
    void update_color_mix(int index, const ColorMixingConfig& config);
    ColorMixingConfig get_color_mix(int index) const;
    std::vector<ColorMixingConfig> get_all_color_mixes() const;

private:
    enum : int {
        ID_APPLY_MIX = wxID_HIGHEST + 3100,
    };

    void on_apply_mix(wxCommandEvent& event);
    void layout_controls();
    void update_preview();
    wxColour mixed_cmy_colour() const;

    wxSpinCtrl* m_spin_material_a { nullptr };
    wxSpinCtrl* m_spin_material_b { nullptr };
    wxSpinCtrl* m_spin_material_c { nullptr };
    wxSpinCtrlDouble* m_flow_ratio_a { nullptr };
    wxSpinCtrlDouble* m_flow_ratio_b { nullptr };
    wxSpinCtrlDouble* m_flow_ratio_c { nullptr };
    wxColourPickerCtrl* m_colour_material_a { nullptr };
    wxColourPickerCtrl* m_colour_material_b { nullptr };
    wxColourPickerCtrl* m_colour_material_c { nullptr };
    wxStaticText* m_preview { nullptr };
    wxPanel* m_mixed_colour { nullptr };
    ApplyFn m_apply_fn;

    std::vector<ColorMixingConfig> m_color_mixes;

    std::array<double, 3> raw_flow_ratios() const;
    std::vector<int> normalized_mix_percentages() const;
    double blended_flow_ratio() const;
    static int round_division(int numerator, int denominator);
};

} // namespace GUI
} // namespace Slic3r

#endif // slic3r_GUI_ColorMixing_hpp_
