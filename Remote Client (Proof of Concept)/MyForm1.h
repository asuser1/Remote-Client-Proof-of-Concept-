#define WIN32_LEAN_AND_MEAN

#pragma once
#include <Windows.h>

#define KEYEVENTF_KEYDOWN 0x0000

namespace Remote_Client__Proof_of_Concept_ {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Collections::Generic;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Net::Sockets;
	using namespace System::Drawing;
	using namespace System::Runtime::Serialization::Formatters::Binary;

	/// <summary>
	/// Summary for external
	/// </summary>
	public ref class external : public System::Windows::Forms::Form
	{
	public:
		external(NetworkStream^ n)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			bf = gcnew BinaryFormatter();
			ns = n;
		}

		void UpdatePicture(Image^ img)
		{
			pictureBox1->Image = img;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~external()
		{
			if (components)
			{
				delete components;
			}
		}
	private: NetworkStream^ ns;
	private: BinaryFormatter^ bf;
	private: System::Windows::Forms::PictureBox^  pictureBox1;

	protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
			this->SuspendLayout();
			// 
			// pictureBox1
			// 
			this->pictureBox1->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->pictureBox1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->pictureBox1->Location = System::Drawing::Point(0, 0);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(1024, 768);
			this->pictureBox1->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pictureBox1->TabIndex = 0;
			this->pictureBox1->TabStop = false;
			this->pictureBox1->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &external::pictureBox1_MouseDown);
			this->pictureBox1->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &external::pictureBox1_MouseMove);
			this->pictureBox1->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &external::pictureBox1_MouseUp);
			// 
			// external
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->AutoSize = true;
			this->ClientSize = System::Drawing::Size(1024, 768);
			this->Controls->Add(this->pictureBox1);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->Name = L"external";
			this->SizeGripStyle = System::Windows::Forms::SizeGripStyle::Hide;
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"external";
			this->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &external::external_KeyDown);
			this->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &external::external_KeyUp);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion
	private:
		int resolutionWidth;
		int resolutionHeight;

	public: 
		void SetResolution(int width, int height) { 
			resolutionWidth = width; 
			resolutionHeight = height; 

			this->Size.Width = width * 0.5;
			this->Size.Height = height * 0.5;
		}

	private: bool IsKeycodeValid(int i)
	{
		bool result = true;

		//Invalid or regional/oem keycodes
		List<int>^ lst = gcnew List<int>(gcnew array<int>
		{ 1, 2, 3, 4, 5, 6, 7, 10, 11, 15, 16, 21, 22, 23, 24, 25, 26, 28, 29, 30, 31, 58, 59, 60, 61, 62, 63, 64,
			94, 95, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140,
			141, 142, 143, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159 });

		if (lst->Contains(i))
			result = false;

		if (i >= 166)
			result = false;

		return result;
	}

	private: System::Void pictureBox1_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) 
	{
		bf->Serialize(ns, gcnew ::MouseMove(((float)resolutionWidth / (float)pictureBox1->Width) * e->X, ((float)resolutionHeight / (float)pictureBox1->Height) * e->Y));
	}

	private: System::Void pictureBox1_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) 
	{
		if (e->Button == System::Windows::Forms::MouseButtons::Left)
		{
			if(e->Clicks == 1)
				bf->Serialize(ns, gcnew ::Message(::Message::MsgType::MSG_TYPE_LBDOWN));
			else if(e->Clicks > 1)
				bf->Serialize(ns, gcnew ::Message(::Message::MsgType::MSG_TYPE_DOUBLECLICK));
		}

		if(e->Button == System::Windows::Forms::MouseButtons::Right)
			bf->Serialize(ns, gcnew ::Message(::Message::MsgType::MSG_TYPE_RBDOWN));
	}

	private: System::Void pictureBox1_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) 
	{
		if (e->Button == System::Windows::Forms::MouseButtons::Left)
			bf->Serialize(ns, gcnew ::Message(::Message::MsgType::MSG_TYPE_LBUP));

		if (e->Button == System::Windows::Forms::MouseButtons::Right)
			bf->Serialize(ns, gcnew ::Message(::Message::MsgType::MSG_TYPE_RBUP));
	}

	private: System::Void external_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) 
	{
		if (!IsKeycodeValid(e->KeyValue))
			return;

		array<int>^ key = gcnew array<int>(4);
		key[0] = e->KeyValue;
		key[1] = MapVirtualKey(e->KeyValue, MAPVK_VK_TO_VSC);
		key[2] = KEYEVENTF_KEYDOWN;
		key[3] = 0;

		bf->Serialize(ns, gcnew ::Message(::Message::MsgType::MSG_TYPE_KBDOWN, key));
	}

	private: System::Void external_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) 
	{
		if (!IsKeycodeValid(e->KeyValue))
			return;

		array<int>^ key = gcnew array<int>(4);
		key[0] = e->KeyValue;
		key[1] = MapVirtualKey(e->KeyValue, MAPVK_VK_TO_VSC);
		key[2] = KEYEVENTF_KEYUP;
		key[3] = 0;

		bf->Serialize(ns, gcnew ::Message(::Message::MsgType::MSG_TYPE_KBUP, key));
	}
};
}
