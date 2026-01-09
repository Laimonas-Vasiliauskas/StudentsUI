#include "pch.h"
#include "Form1.h"

#include <fstream>
#include <random>
#include <string>
#include <functional>
#include <stdexcept>
#include <algorithm>
#include <sstream>
#include <vector>
#include <iomanip>

#include <msclr/gcroot.h>

using namespace System;
using namespace System::Windows::Forms;
using namespace System::IO;
using namespace System::Runtime::InteropServices;
using namespace System::Threading;

namespace CppCLRWinFormsProject {

    // -------------------- Helpers (String^ -> std::string) --------------------
    static std::string ToStdString(String^ s)
    {
        if (s == nullptr) return std::string();
        IntPtr p = Marshal::StringToHGlobalAnsi(s);
        std::string r = static_cast<char*>(p.ToPointer());
        Marshal::FreeHGlobal(p);
        return r;
    }
    static String^ SafeLine(array<String^>^ lines, int i)
    {
        if (lines == nullptr) return "";
        if (i < 0 || i >= lines->Length) return "";
        return lines[i] == nullptr ? "" : lines[i];
    }

    void SaveAllTextBoxesToFile(String^ path,
        TextBox^ students, TextBox^ homework, TextBox^ exam, TextBox^ rezult)
    {
        auto sLines = students->Lines;
        auto hLines = homework->Lines;
        auto eLines = exam->Lines;
        auto rLines = rezult->Lines;

        int n = 0;
        n = Math::Max(n, sLines->Length);
        n = Math::Max(n, hLines->Length);
        n = Math::Max(n, eLines->Length);
        n = Math::Max(n, rLines->Length);

        System::Text::StringBuilder^ sb = gcnew System::Text::StringBuilder();

        // header
        sb->AppendLine("Students\tHomeWork\tExam\tRezults");

        for (int i = 0; i < n; i++)
        {
            String^ s = SafeLine(sLines, i);
            String^ h = SafeLine(hLines, i);
            String^ e = SafeLine(eLines, i);
            String^ r = SafeLine(rLines, i);

            // jei eilutė visiškai tuščia – praleidžiam (nebūtina, bet tvarkingiau)
            if (String::IsNullOrWhiteSpace(s) &&
                String::IsNullOrWhiteSpace(h) &&
                String::IsNullOrWhiteSpace(e) &&
                String::IsNullOrWhiteSpace(r))
                continue;

            // TAB-separated
            sb->Append(s); sb->Append("\t");
            sb->Append(h); sb->Append("\t");
            sb->Append(e); sb->Append("\t");
            sb->Append(r);
            sb->AppendLine();
        }

        File::WriteAllText(path, sb->ToString());
    }
    // -------------------- Avg/Median --------------------
    static double Avg(const std::vector<int>& v)
    {
        if (v.empty()) return 0.0;
        double sum = 0.0;
        for (int x : v) sum += x;
        return sum / v.size();
    }
    static double Med(std::vector<int> v)
    {
        if (v.empty()) return 0.0;
        std::sort(v.begin(), v.end());
        size_t n = v.size();
        if (n % 2 == 1) return v[n / 2];
        return (v[n / 2 - 1] + v[n / 2]) / 2.0;
    }

    // -------------------- Pretty TXT columns --------------------
    static void WriteHeader(std::ofstream& out, const std::string& resultHeader)
    {
        const int W_NAME = 25;
        const int W_ND = 18;
        const int W_EX = 6;

        out << std::left
            << std::setw(W_NAME) << "Vardas Pavarde"
            << std::setw(W_ND) << "ND"
            << std::setw(W_EX) << "Exam"
            << resultHeader
            << "\n";
    }

    static void WriteRow(std::ofstream& out,
        const std::string& name,
        const std::string& ndStr,
        int egz,
        double galutinis)
    {
        const int W_NAME = 25;
        const int W_ND = 18;
        const int W_EX = 6;

        std::ostringstream rez;
        rez << std::fixed << std::setprecision(2) << galutinis;

        out << std::left
            << std::setw(W_NAME) << name
            << std::setw(W_ND) << ndStr
            << std::setw(W_EX) << std::to_string(egz)
            << rez.str()
            << "\n";
    }

    // -------------------- Generator (CPP side) --------------------
    static void GenerateStudentsFile(
        const std::string& filename,
        size_t count,
        int fileIndex,
        const std::function<void(int, int)>& onProgress)
    {
        std::ofstream out(filename, std::ios::out | std::ios::trunc);
        if (!out) throw std::runtime_error("Cannot open output file");

        std::mt19937 gen(123);
        std::uniform_int_distribution<int> d(1, 10);

        const int W_NAME = 25;
        const int W_ND = 18;
        const int W_EX = 6;

        out << std::left
            << std::setw(W_NAME) << "Vardas Pavarde"
            << std::setw(W_ND) << "ND"
            << std::setw(W_EX) << "Exam"
            << "\n";

        const size_t step = (count >= 100000) ? (count / 100) : 1000;

        for (size_t i = 1; i <= count; i++)
        {
            std::string name = "Vardas" + std::to_string(i) + " Pavarde" + std::to_string(i);

            std::ostringstream nd;
            for (int j = 0; j < 5; j++) {
                nd << d(gen);
                if (j < 4) nd << ' ';
            }

            int egz = d(gen);

            out << std::left
                << std::setw(W_NAME) << name
                << std::setw(W_ND) << nd.str()
                << std::setw(W_EX) << std::to_string(egz)
                << "\n";

            if (onProgress && (i % step == 0 || i == count))
            {
                int percent = (int)((i * 100.0) / (double)count);
                onProgress(fileIndex, percent);
            }
        }
    }

    // MUST be in same namespace as declaration in Form1.h
    void GenerateAllStudentFilesWithProgress(const std::function<void(int, int)>& onProgress)
    {
        struct Item { const char* name; size_t n; };
        Item items[] = {
            {"studentai_1000.txt",     1000},
            {"studentai_10000.txt",    10000},
            {"studentai_100000.txt",   100000},
            {"studentai_1000000.txt",  1000000},
            {"studentai_10000000.txt", 10000000}
        };

        for (int i = 0; i < 5; i++)
        {
            if (onProgress) onProgress(i, 0);
            GenerateStudentsFile(items[i].name, items[i].n, i, onProgress);
            if (onProgress) onProgress(i, 100);
        }
    }

    // -------------------- UI Progress callback () --------------------
    static msclr::gcroot<Form1^> g_form;

    static void ProgressCallback(int fileIndex, int percent)
    {
        Form1^ f = g_form;
        if (f == nullptr) return;

        if (percent < 0) percent = 0;
        if (percent > 100) percent = 100;

        f->Invoke(gcnew Action<int, int>(f, &Form1::UpdateProgress), fileIndex, percent);
    }

    // -------------------- Form1 methods --------------------
    void Form1::UpdateProgress(int fileIndex, int percent)
    {
        array<String^>^ names = gcnew array<String^>{
            "studentai_1000.txt",
                "studentai_10000.txt",
                "studentai_100000.txt",
                "studentai_1000000.txt",
                "studentai_10000000.txt"
        };

        if (fileIndex < 0) fileIndex = 0;
        if (fileIndex >= names->Length) fileIndex = names->Length - 1;
        if (percent < 0) percent = 0;
        if (percent > 100) percent = 100;

        progressBar1->Value = percent;
        progressLabel->Text = String::Format("Generating {0}: {1}%", names[fileIndex], percent);
    }

    void Form1::OnGenerateDone()
    {
        this->Cursor = Cursors::Default;
        RandomGenBtn->Enabled = true;
        progressBar1->Value = 100;
        progressLabel->Text = "Done!";
        MessageBox::Show("Files Generated!", "OK", MessageBoxButtons::OK, MessageBoxIcon::Information);
    }

    void Form1::OnGenerateError()
    {
        this->Cursor = Cursors::Default;
        RandomGenBtn->Enabled = true;
        progressLabel->Text = "Error generating files!";
        MessageBox::Show("Generate Error.", "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
    }

    void Form1::GenerateFilesWorker()
    {
        try
        {
            g_form = this;
            GenerateAllStudentFilesWithProgress(ProgressCallback);
            g_form = nullptr;

            this->Invoke(gcnew Action(this, &Form1::OnGenerateDone));
        }
        catch (...)
        {
            g_form = nullptr;
            this->Invoke(gcnew Action(this, &Form1::OnGenerateError));
        }
    }

    // -------------------- Split UI helpers --------------------
    void Form1::UpdateSplitProgress(int percent)
    {
        if (percent < 0) percent = 0;
        if (percent > 100) percent = 100;

        progressBar1->Value = percent;
        progressLabel->Text = String::Format("Splitting... {0}%", percent);
    }

    void Form1::OnSplitDone(String^ outWeak, String^ outStrong)
    {
        this->Cursor = Cursors::Default;
        progressBar1->Value = 100;
        progressLabel->Text = "Done!";

        MessageBox::Show(
            "Split is done!\n\nFiles created:\n" + outWeak + "\n" + outStrong,
            "OK", MessageBoxButtons::OK, MessageBoxIcon::Information);
    }

    void Form1::OnSplitError(String^ msg)
    {
        this->Cursor = Cursors::Default;
        progressLabel->Text = "Error!";
        MessageBox::Show(msg, "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
    }

    // Worker
    void Form1::SplitFilesWorker(String^ inputPath, bool useMedian)
    {
        try
        {
            String^ dir = Path::GetDirectoryName(inputPath);
            String^ outWeakManaged = Path::Combine(dir, "vargsiukai.txt");
            String^ outStrongManaged = Path::Combine(dir, "kietiakiai.txt");

            std::string inFile = ToStdString(inputPath);
            std::string outWeak = ToStdString(outWeakManaged);
            std::string outStrong = ToStdString(outStrongManaged);

            std::ifstream in(inFile);
            if (!in) throw std::runtime_error("Unable to open input file.");

            std::ofstream weak(outWeak, std::ios::out | std::ios::trunc);
            std::ofstream strong(outStrong, std::ios::out | std::ios::trunc);
            if (!weak || !strong) throw std::runtime_error("Unable to create output files.");

            std::string resultHeader = useMedian ? "Mediana" : "Average";
            WriteHeader(weak, resultHeader);
            WriteHeader(strong, resultHeader);

            // progress by bytes
            in.seekg(0, std::ios::end);
            std::streampos total = in.tellg();
            in.seekg(0, std::ios::beg);

            std::string line;
            while (std::getline(in, line))
            {
                if (line.empty()) continue;

                std::streampos pos = in.tellg();
                if (total > 0 && pos > 0)
                {
                    int pct = (int)((pos * 100.0) / total);
                    this->Invoke(gcnew Action<int>(this, &Form1::UpdateSplitProgress), pct);
                }

                std::istringstream ss(line);
                std::string vardas, pavarde;
                if (!(ss >> vardas >> pavarde)) continue;

                // skip header
                if (vardas == "Vardas" && pavarde == "Pavarde") continue;

                std::vector<std::string> rest;
                std::string token;
                while (ss >> token) rest.push_back(token);
                if (rest.size() < 1) continue;

                int egz = 0;
                try { egz = std::stoi(rest.back()); }
                catch (...) { continue; }

                std::vector<int> nd;
                for (size_t i = 0; i + 1 < rest.size(); i++) {
                    try { nd.push_back(std::stoi(rest[i])); }
                    catch (...) {}
                }

                std::ostringstream ndStr;
                for (size_t i = 0; i < nd.size(); i++) {
                    ndStr << nd[i];
                    if (i + 1 < nd.size()) ndStr << ' ';
                }

                double ndRes = useMedian ? Med(nd) : Avg(nd);
                double galutinis = 0.4 * ndRes + 0.6 * egz;

                std::string name = vardas + " " + pavarde;

                if (galutinis < 5.0) WriteRow(weak, name, ndStr.str(), egz, galutinis);
                else                 WriteRow(strong, name, ndStr.str(), egz, galutinis);
            }

            this->Invoke(gcnew Action<String^, String^>(this, &Form1::OnSplitDone),
                outWeakManaged, outStrongManaged);
        }
        catch (const std::exception& ex)
        {
            this->Invoke(gcnew Action<String^>(this, &Form1::OnSplitError), gcnew String(ex.what()));
        }
        catch (...)
        {
            this->Invoke(gcnew Action<String^>(this, &Form1::OnSplitError), "Unknown error.");
        }
    }

    // thread start 
    void Form1::SplitThreadStart()
    {
        this->SplitFilesWorker(_splitInput, _splitUseMedian);
    }

    double Form1::ComputeNdResult(const std::vector<int>& nd, bool useMedian)
    {
        if (useMedian) return Med(nd);
        return Avg(nd);
    }
    // -------------------- Event handlers --------------------
    System::Void Form1::OpenFileBtn_Click(System::Object^ sender, System::EventArgs^ e)
    {
        openFileDialog1->Filter = "Text files (*.txt)|*.txt|All files (*.*)|*.*";
        if (openFileDialog1->ShowDialog() != System::Windows::Forms::DialogResult::OK)
            return;

        _currentFile = openFileDialog1->FileName;
        progressLabel->Text = "Opened: " + _currentFile;

        StudentsNameTextBox->Clear();
        HomeWorkTextBox->Clear();
        ExamTextBox->Clear();
        RezultTextBox->Clear();

        try
        {
            array<String^>^ lines = File::ReadAllLines(_currentFile);

            System::Text::StringBuilder^ sbNames = gcnew System::Text::StringBuilder();
            System::Text::StringBuilder^ sbNd = gcnew System::Text::StringBuilder();
            System::Text::StringBuilder^ sbExam = gcnew System::Text::StringBuilder();

            for each (String ^ line in lines)
            {
                if (String::IsNullOrWhiteSpace(line)) continue;

                array<String^>^ parts = line->Split(
                    gcnew array<wchar_t>{ ' ', '\t' },
                    StringSplitOptions::RemoveEmptyEntries
                );

                if (parts->Length >= 2 && parts[0] == "Vardas" && parts[1] == "Pavarde")
                    continue;

                if (parts->Length < 3) continue;

                sbNames->Append(parts[0] + " " + parts[1]);
                sbNames->AppendLine();

                sbExam->Append(parts[parts->Length - 1]);
                sbExam->AppendLine();

                System::Text::StringBuilder^ oneNd = gcnew System::Text::StringBuilder();
                for (int i = 2; i < parts->Length - 1; i++)
                {
                    oneNd->Append(parts[i]);
                    if (i < parts->Length - 2) oneNd->Append(" ");
                }
                sbNd->Append(oneNd->ToString());
                sbNd->AppendLine();
            }

            StudentsNameTextBox->Text = sbNames->ToString();
            HomeWorkTextBox->Text = sbNd->ToString();
            ExamTextBox->Text = sbExam->ToString();

        }
        catch (Exception^ ex)
        {
            MessageBox::Show("Unable to read file: " + ex->Message);
        }
    }

    System::Void Form1::SaveFileBtn_Click(System::Object^ sender, System::EventArgs^ e)
    {
        if (_currentFile == nullptr || _currentFile->Length == 0)
        {
            SaveAsBtn_Click(sender, e);
            return;
        }

        try
        {
            SaveAllTextBoxesToFile(_currentFile,
                StudentsNameTextBox, HomeWorkTextBox, ExamTextBox, RezultTextBox);

            MessageBox::Show("Saved: " + _currentFile);
        }
        catch (Exception^ ex)
        {
            MessageBox::Show("Save error: " + ex->Message);
        }
    }

    System::Void Form1::SaveAsBtn_Click(System::Object^ sender, System::EventArgs^ e)
    {
        saveFileDialog1->Filter = "Text files (*.txt)|*.txt|All files (*.*)|*.*";
        if (saveFileDialog1->ShowDialog() != System::Windows::Forms::DialogResult::OK)
            return;

        _currentFile = saveFileDialog1->FileName;

        try
        {
            SaveAllTextBoxesToFile(_currentFile,
                StudentsNameTextBox, HomeWorkTextBox, ExamTextBox, RezultTextBox);

            MessageBox::Show("Saved as: " + _currentFile);
        }
        catch (Exception^ ex)
        {
            MessageBox::Show("Save As error: " + ex->Message);
        }
    }

    System::Void Form1::RandomGenBtn_Click(System::Object^ sender, System::EventArgs^ e)
    {
        RandomGenBtn->Enabled = false;
        this->Cursor = Cursors::WaitCursor;

        progressBar1->Value = 0;
        progressLabel->Text = "Generating...";

        Thread^ t = gcnew Thread(gcnew ThreadStart(this, &Form1::GenerateFilesWorker));
        t->IsBackground = true;
        t->Start();
    }

    System::Void Form1::SplitBtn_Click(System::Object^ sender, System::EventArgs^ e)
    {
        if (_currentFile == nullptr || _currentFile->Length == 0) {
            MessageBox::Show("First open a file.");
            return;
        }

        if (!AverageRadioBtn->Checked && !MedianaRadioBtn->Checked) {
            MessageBox::Show("Select Average or Median.");
            return;
        }

        _splitInput = _currentFile;
        _splitUseMedian = MedianaRadioBtn->Checked;

        this->Cursor = Cursors::WaitCursor;
        progressBar1->Minimum = 0;
        progressBar1->Maximum = 100;
        progressBar1->Value = 0;
        progressLabel->Text = "Splitting...";

        Thread^ t = gcnew Thread(gcnew ThreadStart(this, &Form1::SplitThreadStart));
        t->IsBackground = true;
        t->Start();
    }


    System::Void Form1::RezultBtn_Click(System::Object^ sender, System::EventArgs^ e)
    {
        if (!AverageRadioBtn->Checked && !MedianaRadioBtn->Checked)
        {
            MessageBox::Show("Choose Average or Median.");
            return;
        }

        bool useMedian = MedianaRadioBtn->Checked;

        array<String^>^ ndLines = HomeWorkTextBox->Lines;
        int n = ndLines->Length;

        if (n == 0 || (n == 1 && String::IsNullOrWhiteSpace(ndLines[0])))
        {
            MessageBox::Show("No ND data (HomeWork). First open a file.");
            return;
        }

        System::Text::StringBuilder^ sb = gcnew System::Text::StringBuilder();

        for (int i = 0; i < n; i++)
        {
            String^ ndStrManaged = ndLines[i];
            if (String::IsNullOrWhiteSpace(ndStrManaged))
                continue;

            // ND -> vector<int>
            std::string ndStd = ToStdString(ndStrManaged);
            std::istringstream iss(ndStd);
            std::vector<int> nd;
            int x;
            while (iss >> x) nd.push_back(x);

            if (nd.empty())
                continue;

            double ndRes = useMedian ? Med(nd) : Avg(nd);

            // tik skaičius (2 skaitmenys po kablelio)
            sb->AppendFormat("{0:F2}", ndRes);
            sb->AppendLine();
        }

        RezultTextBox->Text = sb->ToString();
    }



    // Menu items -> reuse buttons
    System::Void Form1::openFileToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
    {
        OpenFileBtn_Click(sender, e);
    }
    System::Void Form1::saveToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
    {
        SaveFileBtn_Click(sender, e);
    }
    System::Void Form1::saveAsToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
    {
        SaveAsBtn_Click(sender, e);
    }
    System::Void Form1::exitToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
    {
        this->Close();
    }

    System::Void Form1::AverageRadioBtn_CheckedChanged(System::Object^ sender, System::EventArgs^ e)
    {
        if (AverageRadioBtn->Checked) progressLabel->Text = "Mode: Average";
    }
    System::Void Form1::MedianaRadioBtn_CheckedChanged(System::Object^ sender, System::EventArgs^ e)
    {
        if (MedianaRadioBtn->Checked) progressLabel->Text = "Mode: Mediana";
    }
    System::Void Form1::helpToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {
        MessageBox::Show(
            "Student file generator and splitter.\n\n"
			"Program created by Laimonas Vasiliauskas.\n"
        );
    }

} // namespace CppCLRWinFormsProject
