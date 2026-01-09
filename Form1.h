#pragma once

#include <functional>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <random>

namespace CppCLRWinFormsProject {

    // CPP generatoriaus deklaracija (implementuota Form1.cpp)
    void GenerateAllStudentFilesWithProgress(const std::function<void(int, int)>& onProgress);

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;

    public ref class Form1 : public System::Windows::Forms::Form
    {
    public:
        Form1(void)
        {
            InitializeComponent();

            // init
            _currentFile = nullptr;
            _splitInput = nullptr;
            _splitUseMedian = false;

            progressLabel->Text = "";
            progressBar1->Minimum = 0;
            progressBar1->Maximum = 100;
            progressBar1->Value = 0;
        }

        ~Form1()
        {
            if (components)
            {
                delete components;
            }
        }

    public:
        // --- UI methods (kviečiami per Invoke iš worker thread) ---
        void UpdateProgress(int fileIndex, int percent);
        void UpdateSplitProgress(int percent);

        void OnSplitDone(System::String^ outWeak, System::String^ outStrong);
        void OnSplitError(System::String^ msg);

        void OnGenerateDone();
        void OnGenerateError();

        // workers / thread entry
        void GenerateFilesWorker();
        void SplitFilesWorker(System::String^ inputPath, bool useMedian);
        void SplitThreadStart();

    private:
        // currently opened file path
        System::String^ _currentFile;

        // split thread params
        System::String^ _splitInput;
    private: System::Windows::Forms::ToolStripMenuItem^ helpToolStripMenuItem;
           bool _splitUseMedian;
        double ComputeNdResult(const std::vector<int>& nd, bool useMedian);

    private:
        // ------------- Controls -------------
        System::Windows::Forms::MenuStrip^ menuStrip1;
        System::Windows::Forms::ToolStripMenuItem^ fileToolStripMenuItem;
        System::Windows::Forms::ToolStripMenuItem^ openFileToolStripMenuItem;
        System::Windows::Forms::ToolStripMenuItem^ saveToolStripMenuItem;
        System::Windows::Forms::ToolStripMenuItem^ saveAsToolStripMenuItem;
        System::Windows::Forms::ToolStripMenuItem^ exitToolStripMenuItem;

        System::Windows::Forms::Button^ OpenFileBtn;
        System::Windows::Forms::Button^ SaveFileBtn;
        System::Windows::Forms::Button^ SaveAsBtn;
        System::Windows::Forms::Button^ RandomGenBtn;
        System::Windows::Forms::Button^ SplitBtn;
        System::Windows::Forms::Button^ RezultBtn;

        System::Windows::Forms::RadioButton^ AverageRadioBtn;
        System::Windows::Forms::RadioButton^ MedianaRadioBtn;

        System::Windows::Forms::TextBox^ StudentsNameTextBox;
        System::Windows::Forms::TextBox^ HomeWorkTextBox;
        System::Windows::Forms::TextBox^ ExamTextBox;
        System::Windows::Forms::TextBox^ RezultTextBox;

        System::Windows::Forms::Label^ label1;
        System::Windows::Forms::Label^ HomeWorkLabel;
        System::Windows::Forms::Label^ ExzamLabel;
        System::Windows::Forms::Label^ RezulLabel;

        System::Windows::Forms::ProgressBar^ progressBar1;
        System::Windows::Forms::Label^ progressLabel;

        System::Windows::Forms::OpenFileDialog^ openFileDialog1;
        System::Windows::Forms::SaveFileDialog^ saveFileDialog1;
        System::Windows::Forms::ContextMenuStrip^ contextMenuStrip1;

        System::ComponentModel::IContainer^ components;

#pragma region Windows Form Designer generated code
        void InitializeComponent(void)
        {
            this->components = (gcnew System::ComponentModel::Container());
            this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
            this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->openFileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->saveToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->saveAsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->exitToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->OpenFileBtn = (gcnew System::Windows::Forms::Button());
            this->SaveFileBtn = (gcnew System::Windows::Forms::Button());
            this->SaveAsBtn = (gcnew System::Windows::Forms::Button());
            this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
            this->saveFileDialog1 = (gcnew System::Windows::Forms::SaveFileDialog());
            this->contextMenuStrip1 = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
            this->RandomGenBtn = (gcnew System::Windows::Forms::Button());
            this->SplitBtn = (gcnew System::Windows::Forms::Button());
            this->RezultBtn = (gcnew System::Windows::Forms::Button());
            this->MedianaRadioBtn = (gcnew System::Windows::Forms::RadioButton());
            this->AverageRadioBtn = (gcnew System::Windows::Forms::RadioButton());
            this->StudentsNameTextBox = (gcnew System::Windows::Forms::TextBox());
            this->HomeWorkTextBox = (gcnew System::Windows::Forms::TextBox());
            this->label1 = (gcnew System::Windows::Forms::Label());
            this->HomeWorkLabel = (gcnew System::Windows::Forms::Label());
            this->ExzamLabel = (gcnew System::Windows::Forms::Label());
            this->RezultTextBox = (gcnew System::Windows::Forms::TextBox());
            this->RezulLabel = (gcnew System::Windows::Forms::Label());
            this->ExamTextBox = (gcnew System::Windows::Forms::TextBox());
            this->progressBar1 = (gcnew System::Windows::Forms::ProgressBar());
            this->progressLabel = (gcnew System::Windows::Forms::Label());
            this->helpToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->menuStrip1->SuspendLayout();
            this->SuspendLayout();
            // 
            // menuStrip1
            // 
            this->menuStrip1->ImageScalingSize = System::Drawing::Size(20, 20);
            this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
                this->fileToolStripMenuItem,
                    this->helpToolStripMenuItem
            });
            this->menuStrip1->Location = System::Drawing::Point(0, 0);
            this->menuStrip1->Name = L"menuStrip1";
            this->menuStrip1->Size = System::Drawing::Size(1626, 28);
            this->menuStrip1->TabIndex = 0;
            this->menuStrip1->Text = L"menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {
                this->openFileToolStripMenuItem,
                    this->saveToolStripMenuItem, this->saveAsToolStripMenuItem, this->exitToolStripMenuItem
            });
            this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
            this->fileToolStripMenuItem->Size = System::Drawing::Size(46, 24);
            this->fileToolStripMenuItem->Text = L"File";
            // 
            // openFileToolStripMenuItem
            // 
            this->openFileToolStripMenuItem->Name = L"openFileToolStripMenuItem";
            this->openFileToolStripMenuItem->Size = System::Drawing::Size(224, 26);
            this->openFileToolStripMenuItem->Text = L"Open File";
            this->openFileToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::openFileToolStripMenuItem_Click);
            // 
            // saveToolStripMenuItem
            // 
            this->saveToolStripMenuItem->Name = L"saveToolStripMenuItem";
            this->saveToolStripMenuItem->Size = System::Drawing::Size(224, 26);
            this->saveToolStripMenuItem->Text = L"Save";
            this->saveToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::saveToolStripMenuItem_Click);
            // 
            // saveAsToolStripMenuItem
            // 
            this->saveAsToolStripMenuItem->Name = L"saveAsToolStripMenuItem";
            this->saveAsToolStripMenuItem->Size = System::Drawing::Size(224, 26);
            this->saveAsToolStripMenuItem->Text = L"Save As";
            this->saveAsToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::saveAsToolStripMenuItem_Click);
            // 
            // exitToolStripMenuItem
            // 
            this->exitToolStripMenuItem->Name = L"exitToolStripMenuItem";
            this->exitToolStripMenuItem->Size = System::Drawing::Size(224, 26);
            this->exitToolStripMenuItem->Text = L"Exit";
            this->exitToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::exitToolStripMenuItem_Click);
            // 
            // OpenFileBtn
            // 
            this->OpenFileBtn->Location = System::Drawing::Point(1207, 65);
            this->OpenFileBtn->Name = L"OpenFileBtn";
            this->OpenFileBtn->Size = System::Drawing::Size(403, 40);
            this->OpenFileBtn->TabIndex = 2;
            this->OpenFileBtn->Text = L"Open file";
            this->OpenFileBtn->UseVisualStyleBackColor = true;
            this->OpenFileBtn->Click += gcnew System::EventHandler(this, &Form1::OpenFileBtn_Click);
            // 
            // SaveFileBtn
            // 
            this->SaveFileBtn->Location = System::Drawing::Point(1207, 111);
            this->SaveFileBtn->Name = L"SaveFileBtn";
            this->SaveFileBtn->Size = System::Drawing::Size(403, 40);
            this->SaveFileBtn->TabIndex = 3;
            this->SaveFileBtn->Text = L"Save";
            this->SaveFileBtn->UseVisualStyleBackColor = true;
            this->SaveFileBtn->Click += gcnew System::EventHandler(this, &Form1::SaveFileBtn_Click);
            // 
            // SaveAsBtn
            // 
            this->SaveAsBtn->Location = System::Drawing::Point(1207, 157);
            this->SaveAsBtn->Name = L"SaveAsBtn";
            this->SaveAsBtn->Size = System::Drawing::Size(403, 40);
            this->SaveAsBtn->TabIndex = 4;
            this->SaveAsBtn->Text = L"Save as";
            this->SaveAsBtn->UseVisualStyleBackColor = true;
            this->SaveAsBtn->Click += gcnew System::EventHandler(this, &Form1::SaveAsBtn_Click);
            // 
            // contextMenuStrip1
            // 
            this->contextMenuStrip1->ImageScalingSize = System::Drawing::Size(20, 20);
            this->contextMenuStrip1->Name = L"contextMenuStrip1";
            this->contextMenuStrip1->Size = System::Drawing::Size(61, 4);
            // 
            // RandomGenBtn
            // 
            this->RandomGenBtn->Location = System::Drawing::Point(1207, 203);
            this->RandomGenBtn->Name = L"RandomGenBtn";
            this->RandomGenBtn->Size = System::Drawing::Size(403, 40);
            this->RandomGenBtn->TabIndex = 5;
            this->RandomGenBtn->Text = L"Random Student Generate";
            this->RandomGenBtn->UseVisualStyleBackColor = true;
            this->RandomGenBtn->Click += gcnew System::EventHandler(this, &Form1::RandomGenBtn_Click);
            // 
            // SplitBtn
            // 
            this->SplitBtn->Location = System::Drawing::Point(1207, 249);
            this->SplitBtn->Name = L"SplitBtn";
            this->SplitBtn->Size = System::Drawing::Size(403, 40);
            this->SplitBtn->TabIndex = 7;
            this->SplitBtn->Text = L"Split Students BAD | GOOD";
            this->SplitBtn->UseVisualStyleBackColor = true;
            this->SplitBtn->Click += gcnew System::EventHandler(this, &Form1::SplitBtn_Click);
            // 
            // RezultBtn
            // 
            this->RezultBtn->Location = System::Drawing::Point(1294, 492);
            this->RezultBtn->Name = L"RezultBtn";
            this->RezultBtn->Size = System::Drawing::Size(316, 46);
            this->RezultBtn->TabIndex = 6;
            this->RezultBtn->Text = L"Rezult";
            this->RezultBtn->UseVisualStyleBackColor = true;
            this->RezultBtn->Click += gcnew System::EventHandler(this, &Form1::RezultBtn_Click);
            // 
            // MedianaRadioBtn
            // 
            this->MedianaRadioBtn->AutoSize = true;
            this->MedianaRadioBtn->Location = System::Drawing::Point(1207, 518);
            this->MedianaRadioBtn->Name = L"MedianaRadioBtn";
            this->MedianaRadioBtn->Size = System::Drawing::Size(81, 20);
            this->MedianaRadioBtn->TabIndex = 7;
            this->MedianaRadioBtn->TabStop = true;
            this->MedianaRadioBtn->Text = L"Mediana";
            this->MedianaRadioBtn->UseVisualStyleBackColor = true;
            this->MedianaRadioBtn->CheckedChanged += gcnew System::EventHandler(this, &Form1::MedianaRadioBtn_CheckedChanged);
            // 
            // AverageRadioBtn
            // 
            this->AverageRadioBtn->AutoSize = true;
            this->AverageRadioBtn->Location = System::Drawing::Point(1208, 492);
            this->AverageRadioBtn->Name = L"AverageRadioBtn";
            this->AverageRadioBtn->Size = System::Drawing::Size(80, 20);
            this->AverageRadioBtn->TabIndex = 8;
            this->AverageRadioBtn->TabStop = true;
            this->AverageRadioBtn->Text = L"Average";
            this->AverageRadioBtn->UseVisualStyleBackColor = true;
            this->AverageRadioBtn->CheckedChanged += gcnew System::EventHandler(this, &Form1::AverageRadioBtn_CheckedChanged);
            // 
            // StudentsNameTextBox
            // 
            this->StudentsNameTextBox->Location = System::Drawing::Point(17, 65);
            this->StudentsNameTextBox->Multiline = true;
            this->StudentsNameTextBox->Name = L"StudentsNameTextBox";
            this->StudentsNameTextBox->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
            this->StudentsNameTextBox->Size = System::Drawing::Size(251, 474);
            this->StudentsNameTextBox->TabIndex = 9;
            this->StudentsNameTextBox->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            // 
            // HomeWorkTextBox
            // 
            this->HomeWorkTextBox->Location = System::Drawing::Point(274, 65);
            this->HomeWorkTextBox->Multiline = true;
            this->HomeWorkTextBox->Name = L"HomeWorkTextBox";
            this->HomeWorkTextBox->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
            this->HomeWorkTextBox->Size = System::Drawing::Size(277, 474);
            this->HomeWorkTextBox->TabIndex = 10;
            this->HomeWorkTextBox->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            // 
            // label1
            // 
            this->label1->AutoSize = true;
            this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Bold));
            this->label1->Location = System::Drawing::Point(80, 40);
            this->label1->Name = L"label1";
            this->label1->Size = System::Drawing::Size(98, 25);
            this->label1->TabIndex = 17;
            this->label1->Text = L"Students";
            // 
            // HomeWorkLabel
            // 
            this->HomeWorkLabel->AutoSize = true;
            this->HomeWorkLabel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Bold));
            this->HomeWorkLabel->Location = System::Drawing::Point(337, 40);
            this->HomeWorkLabel->Name = L"HomeWorkLabel";
            this->HomeWorkLabel->Size = System::Drawing::Size(119, 25);
            this->HomeWorkLabel->TabIndex = 18;
            this->HomeWorkLabel->Text = L"HomeWork";
            // 
            // ExzamLabel
            // 
            this->ExzamLabel->AutoSize = true;
            this->ExzamLabel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Bold));
            this->ExzamLabel->Location = System::Drawing::Point(552, 40);
            this->ExzamLabel->Name = L"ExzamLabel";
            this->ExzamLabel->Size = System::Drawing::Size(66, 25);
            this->ExzamLabel->TabIndex = 23;
            this->ExzamLabel->Text = L"Exam";
            // 
            // RezultTextBox
            // 
            this->RezultTextBox->Location = System::Drawing::Point(624, 64);
            this->RezultTextBox->Multiline = true;
            this->RezultTextBox->Name = L"RezultTextBox";
            this->RezultTextBox->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
            this->RezultTextBox->Size = System::Drawing::Size(577, 475);
            this->RezultTextBox->TabIndex = 24;
            this->RezultTextBox->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            // 
            // RezulLabel
            // 
            this->RezulLabel->AutoSize = true;
            this->RezulLabel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 13.8F));
            this->RezulLabel->Location = System::Drawing::Point(853, 36);
            this->RezulLabel->Name = L"RezulLabel";
            this->RezulLabel->Size = System::Drawing::Size(92, 29);
            this->RezulLabel->TabIndex = 25;
            this->RezulLabel->Text = L"Rezults";
            // 
            // ExamTextBox
            // 
            this->ExamTextBox->Location = System::Drawing::Point(557, 65);
            this->ExamTextBox->Multiline = true;
            this->ExamTextBox->Name = L"ExamTextBox";
            this->ExamTextBox->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
            this->ExamTextBox->Size = System::Drawing::Size(61, 473);
            this->ExamTextBox->TabIndex = 15;
            this->ExamTextBox->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            // 
            // progressBar1
            // 
            this->progressBar1->Location = System::Drawing::Point(17, 546);
            this->progressBar1->Name = L"progressBar1";
            this->progressBar1->Size = System::Drawing::Size(251, 23);
            this->progressBar1->TabIndex = 26;
            // 
            // progressLabel
            // 
            this->progressLabel->AutoSize = true;
            this->progressLabel->Location = System::Drawing::Point(275, 552);
            this->progressLabel->Name = L"progressLabel";
            this->progressLabel->Size = System::Drawing::Size(0, 16);
            this->progressLabel->TabIndex = 27;
            // 
            // helpToolStripMenuItem
            // 
            this->helpToolStripMenuItem->Name = L"helpToolStripMenuItem";
            this->helpToolStripMenuItem->Size = System::Drawing::Size(55, 24);
            this->helpToolStripMenuItem->Text = L"Help";
            this->helpToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::helpToolStripMenuItem_Click);
            // 
            // Form1
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(1626, 578);
            this->Controls->Add(this->progressLabel);
            this->Controls->Add(this->progressBar1);
            this->Controls->Add(this->RezulLabel);
            this->Controls->Add(this->RezultTextBox);
            this->Controls->Add(this->ExzamLabel);
            this->Controls->Add(this->HomeWorkLabel);
            this->Controls->Add(this->label1);
            this->Controls->Add(this->ExamTextBox);
            this->Controls->Add(this->HomeWorkTextBox);
            this->Controls->Add(this->StudentsNameTextBox);
            this->Controls->Add(this->MedianaRadioBtn);
            this->Controls->Add(this->AverageRadioBtn);
            this->Controls->Add(this->RezultBtn);
            this->Controls->Add(this->SplitBtn);
            this->Controls->Add(this->RandomGenBtn);
            this->Controls->Add(this->SaveAsBtn);
            this->Controls->Add(this->SaveFileBtn);
            this->Controls->Add(this->OpenFileBtn);
            this->Controls->Add(this->menuStrip1);
            this->MainMenuStrip = this->menuStrip1;
            this->Name = L"Form1";
            this->Text = L"Form1";
            this->menuStrip1->ResumeLayout(false);
            this->menuStrip1->PerformLayout();
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion

    private:
        // -------- Event handlers declarations (implement in Form1.cpp) --------
        System::Void OpenFileBtn_Click(System::Object^ sender, System::EventArgs^ e);
        System::Void SaveFileBtn_Click(System::Object^ sender, System::EventArgs^ e);
        System::Void SaveAsBtn_Click(System::Object^ sender, System::EventArgs^ e);
        System::Void RandomGenBtn_Click(System::Object^ sender, System::EventArgs^ e);
        System::Void SplitBtn_Click(System::Object^ sender, System::EventArgs^ e);
        System::Void RezultBtn_Click(System::Object^ sender, System::EventArgs^ e);

        System::Void openFileToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e);
        System::Void saveToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e);
        System::Void saveAsToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e);
        System::Void exitToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e);

        System::Void AverageRadioBtn_CheckedChanged(System::Object^ sender, System::EventArgs^ e);
        System::Void MedianaRadioBtn_CheckedChanged(System::Object^ sender, System::EventArgs^ e);
        System::Void helpToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e);
};
}
