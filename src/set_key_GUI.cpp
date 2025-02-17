#define UNICODE
#include <wx/wx.h>
#include <fstream>
#include <string>

using namespace std;

enum
{
    ID_TEXT_CONTROL = 15
};

int convert_key_code_to_win_key_code(int wxKeyCode)
{
    static unordered_map<int, int> keyMap = {
        {WXK_BACK, VK_BACK}, {WXK_TAB, VK_TAB}, {WXK_RETURN, VK_RETURN}, {WXK_ESCAPE, VK_ESCAPE}, {WXK_SPACE, VK_SPACE}, {WXK_DELETE, VK_DELETE}, {WXK_SHIFT, VK_SHIFT}, {WXK_CONTROL, VK_CONTROL}, {WXK_ALT, VK_MENU}, {WXK_CAPITAL, VK_CAPITAL}, {WXK_F1, VK_F1}, {WXK_F2, VK_F2}, {WXK_F3, VK_F3}, {WXK_F4, VK_F4}, {WXK_F5, VK_F5}, {WXK_F6, VK_F6}, {WXK_F7, VK_F7}, {WXK_F8, VK_F8}, {WXK_F9, VK_F9}, {WXK_F10, VK_F10}, {WXK_F11, VK_F11}, {WXK_F12, VK_F12}, {WXK_NUMPAD0, VK_NUMPAD0}, {WXK_NUMPAD1, VK_NUMPAD1}, {WXK_NUMPAD2, VK_NUMPAD2}, {WXK_NUMPAD3, VK_NUMPAD3}, {WXK_NUMPAD4, VK_NUMPAD4}, {WXK_NUMPAD5, VK_NUMPAD5}, {WXK_NUMPAD6, VK_NUMPAD6}, {WXK_NUMPAD7, VK_NUMPAD7}, {WXK_NUMPAD8, VK_NUMPAD8}, {WXK_NUMPAD9, VK_NUMPAD9}, {WXK_ADD, VK_ADD}, {WXK_SUBTRACT, VK_SUBTRACT}, {WXK_MULTIPLY, VK_MULTIPLY}, {WXK_DIVIDE, VK_DIVIDE}};

    if (keyMap.find(wxKeyCode) != keyMap.end())
    {
        return keyMap[wxKeyCode];
    }

    if ((wxKeyCode >= 'A' && wxKeyCode <= 'Z') || (wxKeyCode >= '0' && wxKeyCode <= '9'))
    {
        return wxKeyCode;
    }

    return 0;
}

void write_file(const string &filename, int key_code)
{
    ofstream out_file(filename);
    out_file << key_code;
    out_file.close();
}

wxFont set_font()
{
    wxFont font(14, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
    return font;
}

class MainFrame : public wxFrame
{
public:
    MainFrame(const wxString &title) : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX))
    {
        SetIcon(wxICON(IDI_ICON1));
        SetBackgroundColour(*wxWHITE);
        CreateStatusBar();

        int textWidth = 250;
        int textHeight = 20;

        int boxWidth = 200;
        int boxHeight = 30;

        wxPanel *panel = new wxPanel(this);
        wxStaticText *text = new wxStaticText(panel, wxID_ANY, "Enter a key to hide desktop icons:", wxDefaultPosition, wxSize(textWidth, textHeight), wxALIGN_CENTER);
        wxTextCtrl *text_control = new wxTextCtrl(panel, ID_TEXT_CONTROL, "", wxDefaultPosition, wxSize(boxWidth, boxHeight), wxTE_CENTER);

        wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
        wxSizerFlags flags = wxSizerFlags().CenterHorizontal().Border(wxALL, 5);

        boxSizer->AddStretchSpacer(1);
        boxSizer->Add(text, 0, wxEXPAND | wxALL, 5);
        boxSizer->Add(text_control, flags);
        boxSizer->AddStretchSpacer(1);
        panel->SetSizer(boxSizer);
        boxSizer->SetSizeHints(this);

        wxFont font = set_font();
        text->SetFont(font);
        text_control->SetFont(font);
        text_control->SetEditable(true);

        text_control->Bind(wxEVT_KEY_DOWN, &MainFrame::OnKeyDown, this);
    }

    wxString assigned_key;

private:
    void OnKeyDown(wxKeyEvent &event)
    {
        wxTextCtrl *text_control = wxDynamicCast(FindWindow(ID_TEXT_CONTROL), wxTextCtrl);
        if (!text_control)
            return;

        int key_code = event.GetKeyCode();
        int windows_key_code = convert_key_code_to_win_key_code(key_code);
        write_file("key.txt", windows_key_code);

        wxChar key = event.GetUnicodeKey();
        wxString message;
        if (key == WXK_NONE)
        {
            switch (key_code)
            {
            case WXK_F1:
                message = "F1 Key Assigned!";
                break;
            case WXK_F2:
                message = "F2 Key Assigned!";
                break;
            case WXK_F3:
                message = "F3 Key Assigned!";
                break;
            case WXK_F4:
                message = "F4 Key Assigned!";
                break;
            case WXK_F5:
                message = "F5 Key Assigned!";
                break;
            case WXK_F6:
                message = "F6 Key Assigned!";
                break;
            case WXK_F7:
                message = "F7 Key Assigned!";
                break;
            case WXK_F8:
                message = "F8 Key Assigned!";
                break;
            case WXK_F9:
                message = "F9 Key Assigned!";
                break;
            case WXK_F10:
                message = "F10 Key Assigned!";
                break;
            case WXK_F11:
                message = "F11 Key Assigned!";
                break;
            case WXK_F12:
                message = "F12 Key Assigned!";
                break;
            case WXK_CAPITAL:
                message = "Caps Lock Assigned!";
                break;
            case WXK_SHIFT:
                message = "Shift Key Assigned!";
                break;
            case WXK_CONTROL:
                message = "Ctrl Key Assigned!";
                break;
            case WXK_ALT:
                message = "Alt Key Assigned!";
                break;
            case WXK_WINDOWS_LEFT:
                message = "Left Windows Key Assigned!";
                break;
            case WXK_WINDOWS_RIGHT:
                message = "Right Windows Key Assigned!";
                break;
            case WXK_MENU:
                message = "Menu Key Assigned!";
                break;
            case WXK_TAB:
                message = "Tab Key Assigned!";
                break;
            case WXK_RETURN:
                message = "Enter Key Assigned!";
                break;
            case WXK_BACK:
                message = "Backspace Key Assigned!";
                break;
            case WXK_ESCAPE:
                message = "Escape Key Assigned!";
                break;
            case WXK_SPACE:
                message = "Spacebar Assigned!";
                break;
            case WXK_DELETE:
                message = "Delete Key Assigned!";
                break;
            case WXK_INSERT:
                message = "Insert Key Assigned!";
                break;
            case WXK_HOME:
                message = "Home Key Assigned!";
                break;
            case WXK_END:
                message = "End Key Assigned!";
                break;
            case WXK_PAGEUP:
                message = "Page Up Key Assigned!";
                break;
            case WXK_PAGEDOWN:
                message = "Page Down Key Assigned!";
                break;
            case WXK_LEFT:
                message = "Left Arrow Key Assigned!";
                break;
            case WXK_RIGHT:
                message = "Right Arrow Key Assigned!";
                break;
            case WXK_UP:
                message = "Up Arrow Key Assigned!";
                break;
            case WXK_DOWN:
                message = "Down Arrow Key Assigned!";
                break;
            case WXK_NUMLOCK:
                message = "Num Lock Assigned!";
                break;
            case WXK_SCROLL:
                message = "Scroll Lock Assigned!";
                break;
            case WXK_PAUSE:
                message = "Pause/Break Key Assigned!";
                break;
            case WXK_PRINT:
                message = "Print Screen Key Assigned!";
                break;
            default:
                message = "Unknown Special Key Assigned!";
                break;
            }
        }
        else
        {
            message = wxString::Format("Key assigned: %c", key);
        }

        wxLogStatus(message);
        text_control->SetValue(message);

        text_control->SetEditable(false);
        event.Skip();
    }
};

class App : public wxApp
{
public:
    bool OnInit() override
    {
        MainFrame *frame = new MainFrame("Hide Desktop Icons");
        frame->SetClientSize(800, 600);
        frame->Center();
        frame->Show();

        return true;
    }
};

wxIMPLEMENT_APP(App);
