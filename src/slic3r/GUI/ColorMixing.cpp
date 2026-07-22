#include "ColorMixing.hpp"

#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/stattext.h>

#include <algorithm>
#include <array>
#include <cmath>

namespace Slic3r {
namespace GUI {

wxBEGIN_EVENT_TABLE(ColorMixingPanel, wxPanel)
    EVT_BUTTON(ID_APPLY_MIX, ColorMixingPanel::on_apply_mix)
wxEND_EVENT_TABLE()

ColorMixingPanel::ColorMixingPanel(wxWindow* parent, wxWindowID id, ApplyFn apply_fn)
    : wxPanel(parent, id)
    , m_apply_fn(std::move(apply_fn))
{
    layout_controls();
}

void ColorMixingPanel::layout_controls()
{
    auto* main_sizer = new wxBoxSizer(wxVERTICAL);

    auto* title = new wxStaticText(this, wxID_ANY, wxT("Mistura de 3 materiais"));
    wxFont title_font = title->GetFont();
    title_font.MakeBold();
    title->SetFont(title_font);
    main_sizer->Add(title, 0, wxALL | wxEXPAND, 6);

    auto* box = new wxStaticBox(this, wxID_ANY, wxT("Quantidade de fluxo por material"));
    auto* mix_sizer = new wxStaticBoxSizer(box, wxVERTICAL);

    auto add_material_row = [this, mix_sizer](const wxString& label, const wxColour& colour, wxColourPickerCtrl*& picker, wxSpinCtrl*& spin, wxSpinCtrlDouble*& flow_spin, int value, double flow_ratio) {
        auto* row = new wxBoxSizer(wxHORIZONTAL);
        auto* text = new wxStaticText(this, wxID_ANY, label);
        picker = new wxColourPickerCtrl(this, wxID_ANY, colour, wxDefaultPosition, wxSize(58, -1));
        spin = new wxSpinCtrl(this, wxID_ANY, wxString::Format(wxT("%d"), value),
                              wxDefaultPosition, wxSize(90, -1), wxSP_ARROW_KEYS, 0, 100, value);
        flow_spin = new wxSpinCtrlDouble(this, wxID_ANY, wxString::Format(wxT("%.2f"), flow_ratio),
                                         wxDefaultPosition, wxSize(92, -1), wxSP_ARROW_KEYS, 0.10, 2.00, flow_ratio, 0.01);
        picker->Bind(wxEVT_COLOURPICKER_CHANGED, [this](wxColourPickerEvent&) { update_preview(); });
        spin->Bind(wxEVT_SPINCTRL, [this](wxCommandEvent&) { update_preview(); });
        spin->Bind(wxEVT_TEXT, [this](wxCommandEvent&) { update_preview(); });
        flow_spin->Bind(wxEVT_SPINCTRLDOUBLE, [this](wxSpinDoubleEvent&) { update_preview(); });
        flow_spin->Bind(wxEVT_TEXT, [this](wxCommandEvent&) { update_preview(); });
        row->Add(text, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 10);
        row->Add(picker, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 10);
        row->Add(spin, 0, wxALIGN_CENTER_VERTICAL);
        row->Add(new wxStaticText(this, wxID_ANY, wxT("unidades")), 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 4);
        row->Add(new wxStaticText(this, wxID_ANY, wxT("Flow Ratio")), 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 4);
        row->Add(flow_spin, 0, wxALIGN_CENTER_VERTICAL);
        mix_sizer->Add(row, 0, wxALL | wxEXPAND, 5);
    };

    add_material_row(wxT("Material A"), wxColour(0, 180, 220), m_colour_material_a, m_spin_material_a, m_flow_ratio_a, 50, 1.00);
    add_material_row(wxT("Material B"), wxColour(220, 0, 140), m_colour_material_b, m_spin_material_b, m_flow_ratio_b, 25, 1.00);
    add_material_row(wxT("Material C"), wxColour(245, 210, 0), m_colour_material_c, m_spin_material_c, m_flow_ratio_c, 25, 1.00);

    main_sizer->Add(mix_sizer, 0, wxALL | wxEXPAND, 6);

    auto* preview_sizer = new wxBoxSizer(wxHORIZONTAL);
    m_mixed_colour = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(70, 34));
    m_preview = new wxStaticText(this, wxID_ANY, wxEmptyString);
    preview_sizer->Add(m_mixed_colour, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 10);
    preview_sizer->Add(m_preview, 0, wxALIGN_CENTER_VERTICAL);
    main_sizer->Add(preview_sizer, 0, wxLEFT | wxRIGHT | wxBOTTOM | wxEXPAND, 6);

    auto* apply_btn = new wxButton(this, ID_APPLY_MIX, wxT("Aplicar no Filament Start G-code"));
    main_sizer->Add(apply_btn, 0, wxALL, 6);

    SetSizer(main_sizer);
    update_preview();
}

void ColorMixingPanel::on_apply_mix(wxCommandEvent&)
{
    const int a = m_spin_material_a->GetValue();
    const int b = m_spin_material_b->GetValue();
    const int c = m_spin_material_c->GetValue();
    const int total = a + b + c;
    const auto percentages = normalized_mix_percentages();
    const double flow_ratio = blended_flow_ratio();

    update_preview();

    if (total <= 0) {
        wxMessageBox(wxT("Informe uma quantidade maior que zero para ao menos um material."),
                     wxT("Mistura de materiais"), wxOK | wxICON_WARNING, this);
        return;
    }

    if (m_apply_fn)
        m_apply_fn(a, b, c, mixed_cmy_colour(), percentages, flow_ratio);
}

void ColorMixingPanel::update_preview()
{
    const int a = m_spin_material_a->GetValue();
    const int b = m_spin_material_b->GetValue();
    const int c = m_spin_material_c->GetValue();
    const int total = a + b + c;
    const auto percentages = normalized_mix_percentages();
    const double flow_ratio = blended_flow_ratio();

    if (m_preview)
        m_preview->SetLabel(wxString::Format(wxT("Bruto: A%d B%d C%d  |  Ajustado: A%d B%d C%d  |  Flow final %.3f"),
                                             a, b, c,
                                             percentages[0], percentages[1], percentages[2],
                                             flow_ratio));

    if (m_mixed_colour) {
        m_mixed_colour->SetBackgroundColour(mixed_cmy_colour());
        m_mixed_colour->Refresh();
    }
}

int ColorMixingPanel::round_division(int numerator, int denominator)
{
    if (denominator <= 0)
        return 0;
    return static_cast<int>((numerator + denominator / 2) / denominator);
}

std::vector<int> ColorMixingPanel::normalized_mix_percentages() const
{
    const std::array<double, 3> weighted_values {
        (m_spin_material_a ? m_spin_material_a->GetValue() : 0) / std::max(0.0001, m_flow_ratio_a ? m_flow_ratio_a->GetValue() : 1.0),
        (m_spin_material_b ? m_spin_material_b->GetValue() : 0) / std::max(0.0001, m_flow_ratio_b ? m_flow_ratio_b->GetValue() : 1.0),
        (m_spin_material_c ? m_spin_material_c->GetValue() : 0) / std::max(0.0001, m_flow_ratio_c ? m_flow_ratio_c->GetValue() : 1.0)
    };

    const double total = std::max(0.0001, weighted_values[0] + weighted_values[1] + weighted_values[2]);
    std::array<double, 3> exact_percentages {
        weighted_values[0] * 100.0 / total,
        weighted_values[1] * 100.0 / total,
        weighted_values[2] * 100.0 / total
    };
    std::array<int, 3> percentages {
        int(std::floor(exact_percentages[0])),
        int(std::floor(exact_percentages[1])),
        int(std::floor(exact_percentages[2]))
    };

    int percent_total = percentages[0] + percentages[1] + percentages[2];
    while (percent_total < 100) {
        size_t index = 0;
        double best_fraction = exact_percentages[0] - percentages[0];
        for (size_t i = 1; i < 3; ++i) {
            const double fraction = exact_percentages[i] - percentages[i];
            if (fraction > best_fraction) {
                best_fraction = fraction;
                index = i;
            }
        }
        ++percentages[index];
        ++percent_total;
    }
    while (percent_total > 100) {
        size_t index = 0;
        double best_fraction = percentages[0] - exact_percentages[0];
        for (size_t i = 1; i < 3; ++i) {
            const double fraction = percentages[i] - exact_percentages[i];
            if (fraction > best_fraction) {
                best_fraction = fraction;
                index = i;
            }
        }
        if (percentages[index] <= 0)
            break;
        --percentages[index];
        --percent_total;
    }

    return {percentages[0], percentages[1], percentages[2]};
}

std::array<double, 3> ColorMixingPanel::raw_flow_ratios() const
{
    return {
        std::max(0.0001, m_flow_ratio_a ? m_flow_ratio_a->GetValue() : 1.0),
        std::max(0.0001, m_flow_ratio_b ? m_flow_ratio_b->GetValue() : 1.0),
        std::max(0.0001, m_flow_ratio_c ? m_flow_ratio_c->GetValue() : 1.0)
    };
}

double ColorMixingPanel::blended_flow_ratio() const
{
    const double a = std::max(0, m_spin_material_a ? m_spin_material_a->GetValue() : 0);
    const double b = std::max(0, m_spin_material_b ? m_spin_material_b->GetValue() : 0);
    const double c = std::max(0, m_spin_material_c ? m_spin_material_c->GetValue() : 0);
    const auto ratios = raw_flow_ratios();

    const double denom = (a / ratios[0]) + (b / ratios[1]) + (c / ratios[2]);
    if (denom <= 0.0)
        return 1.0;

    return (a + b + c) / denom;
}

wxColour ColorMixingPanel::mixed_cmy_colour() const
{
    const double weight_a = std::max(0, m_spin_material_a ? m_spin_material_a->GetValue() : 0) / std::max(0.0001, m_flow_ratio_a ? m_flow_ratio_a->GetValue() : 1.0);
    const double weight_b = std::max(0, m_spin_material_b ? m_spin_material_b->GetValue() : 0) / std::max(0.0001, m_flow_ratio_b ? m_flow_ratio_b->GetValue() : 1.0);
    const double weight_c = std::max(0, m_spin_material_c ? m_spin_material_c->GetValue() : 0) / std::max(0.0001, m_flow_ratio_c ? m_flow_ratio_c->GetValue() : 1.0);
    const double total = std::max(0.0001, weight_a + weight_b + weight_c);

    const wxColour colour_a = m_colour_material_a ? m_colour_material_a->GetColour() : wxColour(0, 180, 220);
    const wxColour colour_b = m_colour_material_b ? m_colour_material_b->GetColour() : wxColour(220, 0, 140);
    const wxColour colour_c = m_colour_material_c ? m_colour_material_c->GetColour() : wxColour(245, 210, 0);

    auto mix_rgb_channel = [total, weight_a, weight_b, weight_c](unsigned char a, unsigned char b, unsigned char c) {
        const int cmy_a = 255 - static_cast<int>(a);
        const int cmy_b = 255 - static_cast<int>(b);
        const int cmy_c = 255 - static_cast<int>(c);
        const double mixed_cmy = (cmy_a * weight_a + cmy_b * weight_b + cmy_c * weight_c) / total;
        return static_cast<unsigned char>(std::clamp(255 - mixed_cmy, 0.0, 255.0));
    };

    return wxColour(mix_rgb_channel(colour_a.Red(), colour_b.Red(), colour_c.Red()),
                    mix_rgb_channel(colour_a.Green(), colour_b.Green(), colour_c.Green()),
                    mix_rgb_channel(colour_a.Blue(), colour_b.Blue(), colour_c.Blue()));
}

void ColorMixingPanel::add_color_mix() {}
void ColorMixingPanel::remove_color_mix() {}
void ColorMixingPanel::update_color_mix(int index, const ColorMixingConfig& config)
{
    if (index >= 0 && index < static_cast<int>(m_color_mixes.size()))
        m_color_mixes[index] = config;
}

ColorMixingConfig ColorMixingPanel::get_color_mix(int index) const
{
    if (index >= 0 && index < static_cast<int>(m_color_mixes.size()))
        return m_color_mixes[index];
    return ColorMixingConfig {};
}

std::vector<ColorMixingConfig> ColorMixingPanel::get_all_color_mixes() const
{
    return m_color_mixes;
}

} // namespace GUI
} // namespace Slic3r
