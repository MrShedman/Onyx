#pragma once

#include <stdio.h>

#include "GL\glew.h"

class Capture
{
public:

	Capture()
		:
		m_open(false)
	{}

	~Capture()
	{
		close();
	}

	void create(int width, int height, int framerate, const std::string& filename)
	{
		m_width = width;
		m_height = height;

		const char* cmd = "ffmpeg -r 30 -f rawvideo -pix_fmt rgba -s 1280x720 -i - "
			"-threads 0 -preset fast -y -pix_fmt yuv420p -crf 21 -vf vflip output.mp4";

		// if we've already opened a file then close it
		if (m_open)
		{
			close();
		}

		// open pipe to ffmpeg's stdin in binary write mode
		ffmpeg = _popen(cmd, "wb");

		m_buffer = new uint32_t[m_width*m_height];

		m_open = true;
	}

	void writeFrame()
	{
		if (m_open)
		{
			glReadPixels(0, 0, m_width, m_height, GL_RGBA, GL_UNSIGNED_BYTE, m_buffer);

			fwrite(m_buffer, sizeof(uint32_t)*m_width*m_height, 1, ffmpeg);
		}
	}

	void close()
	{
		if (m_open)
		{
			_pclose(ffmpeg);

			delete m_buffer;
		}
	}

private:

	bool m_open;

	FILE* ffmpeg;

	int m_width;
	int m_height;

	uint32_t* m_buffer;
};