#pragma once

class Assets
{
private:
	Array<Font>	fonts;
	Array<std::pair<String, Audio>>		audios;
	Array<std::pair<String, Texture>>	textures;

	Font	errorFont;
	Audio	errorAudio;
	Texture	errorTexture;

public:
	Assets()
		: errorFont(16)
		, errorAudio()
		, errorTexture()
	{

		auto imageContents = FileSystem::DirectoryContents(U"assets/image/");
		auto audioContents = FileSystem::DirectoryContents(U"assets/audio/");

		Logger << U"---Textureの読み込み開始---";
		for (auto& ic : imageContents) {
			if (FileSystem::IsFile(ic))
			{
				Texture texture(ic);

				if (texture)
				{
					String text = ic.removed(FileSystem::CurrentPath() + U"assets/image/");
					textures.emplace_back(text, texture);
					Logger << text << U"を読み込みました";
				}
			}
		}

		Logger << U"---Audioの読み込み開始---";
		for (auto& ac : audioContents) {
			if (FileSystem::IsFile(ac))
			{
				Audio audio(ac);

				if (audio)
				{
					String text = ac.removed(FileSystem::CurrentPath() + U"assets/audio/");
					audios.emplace_back(text, audio);
					Logger << text << U"を読み込みました";
				}
			}
		}
	}

	Font&	font(int _size) {

		while (_size >= int(fonts.size())) fonts.emplace_back(int(fonts.size()));

		return fonts[_size];
	}
	Texture&	texture(const String& _name) {

		for (auto& t : textures) {
			if (_name == t.first) return t.second;
		}

		return errorTexture;
	}
	Audio&	audio(const String& _name) {

		for (auto& a : audios) {
			if (_name == a.first) return a.second;
		}

		return errorAudio;
	}
};