#include <gdextension_interface.h>
#include <godot_cpp/godot.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/templates/vector.hpp>
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/resource_uid.hpp>
#include <godot_cpp/classes/audio_stream_ogg_vorbis.hpp>
#include <godot_cpp/classes/ogg_packet_sequence.hpp>
using namespace godot;

#include "resource_loader_ogg_vorbis.h"

#include <ogg/ogg.h>
//#include <vorbis/codec.h>
#include "vorbis_min.h"

ResourceLoaderOggVorbis *ResourceLoaderOggVorbis::singleton = nullptr;

// Basically a copy of ResourceImporterOggVorbis::import_ogg_vorbis
Ref<Resource> ResourceLoaderOggVorbis::load(const String &p_path, const String &p_original_path, Error *r_error, bool p_use_sub_threads, float *r_progress, CacheMode p_cache_mode) {
	if (r_error) {
		*r_error = ERR_CANT_OPEN;
	}

	Ref<FileAccess> f = FileAccess::open(p_path, FileAccess::READ);
	ERR_FAIL_COND_V_MSG(f.is_null(), Ref<Resource>(), "Cannot open file '" + p_path + "'.");

	uint64_t len = f->get_length();

	Vector<uint8_t> file_data;
	file_data.resize(len);
	uint8_t *w = file_data.ptrw();

	f->get_buffer(w, len);

	Ref<AudioStreamOggVorbis> ogg_vorbis_stream;
	ogg_vorbis_stream.instantiate();

	Ref<OggPacketSequence> ogg_packet_sequence;
	ogg_packet_sequence.instantiate();

	ogg_stream_state stream_state;
	ogg_sync_state sync_state;
	ogg_page page;
	ogg_packet packet;
	bool initialized_stream = false;

	ogg_sync_init(&sync_state);
	int err;
	size_t cursor = 0;
	size_t packet_count = 0;
	PackedInt64Array page_granule_positions;
	TypedArray<Array> page_data;
	bool done = false;
	while (!done) {
		err = ogg_sync_check(&sync_state);
		ERR_FAIL_COND_V_MSG(err != 0, Ref<AudioStreamOggVorbis>(), "Ogg sync error " + itos(err));
		while (ogg_sync_pageout(&sync_state, &page) != 1) {
			if (cursor >= len) {
				done = true;
				break;
			}
			err = ogg_sync_check(&sync_state);
			ERR_FAIL_COND_V_MSG(err != 0, Ref<AudioStreamOggVorbis>(), "Ogg sync error " + itos(err));
			char *sync_buf = ogg_sync_buffer(&sync_state, OGG_SYNC_BUFFER_SIZE);
			err = ogg_sync_check(&sync_state);
			ERR_FAIL_COND_V_MSG(err != 0, Ref<AudioStreamOggVorbis>(), "Ogg sync error " + itos(err));
			ERR_FAIL_COND_V(cursor > len, Ref<AudioStreamOggVorbis>());
			size_t copy_size = len - cursor;
			if (copy_size > OGG_SYNC_BUFFER_SIZE) {
				copy_size = OGG_SYNC_BUFFER_SIZE;
			}
			memcpy(sync_buf, &file_data[cursor], copy_size);
			ogg_sync_wrote(&sync_state, copy_size);
			cursor += copy_size;
			err = ogg_sync_check(&sync_state);
			ERR_FAIL_COND_V_MSG(err != 0, Ref<AudioStreamOggVorbis>(), "Ogg sync error " + itos(err));
		}
		if (done) {
			break;
		}
		err = ogg_sync_check(&sync_state);
		ERR_FAIL_COND_V_MSG(err != 0, Ref<AudioStreamOggVorbis>(), "Ogg sync error " + itos(err));

		// Have a page now.
		if (!initialized_stream) {
			if (ogg_stream_init(&stream_state, ogg_page_serialno(&page))) {
				ERR_FAIL_V_MSG(Ref<AudioStreamOggVorbis>(), "Failed allocating memory for Ogg Vorbis stream.");
			}
			initialized_stream = true;
		}
		ogg_stream_pagein(&stream_state, &page);
		err = ogg_stream_check(&stream_state);
		ERR_FAIL_COND_V_MSG(err != 0, Ref<AudioStreamOggVorbis>(), "Ogg stream error " + itos(err));
		int desync_iters = 0;

		TypedArray<PackedByteArray> packet_data;
		int64_t granule_pos = 0;

		while (true) {
			err = ogg_stream_packetout(&stream_state, &packet);
			if (err == -1) {
				// According to the docs this is usually recoverable, but don't sit here spinning forever.
				desync_iters++;
				ERR_FAIL_COND_V_MSG(desync_iters > 100, Ref<AudioStreamOggVorbis>(), "Packet sync issue during Ogg import");
				continue;
			} else if (err == 0) {
				// Not enough data to fully reconstruct a packet. Go on to the next page.
				break;
			}
			if (packet_count == 0 && vorbis_synthesis_idheader(&packet) == 0) {
				UtilityFunctions::print_verbose("Found a non-vorbis-header packet in a header position");
				// Clearly this logical stream is not a vorbis stream, so destroy it and try again with the next page.
				if (initialized_stream) {
					ogg_stream_clear(&stream_state);
					initialized_stream = false;
				}
				break;
			}
			granule_pos = packet.granulepos;

			PackedByteArray data;
			data.resize(packet.bytes);
			memcpy(data.ptrw(), packet.packet, packet.bytes);
			packet_data.push_back(data);
			packet_count++;
		}
		if (initialized_stream) {
			//ogg_packet_sequence->push_page(granule_pos, packet_data);
			page_granule_positions.push_back(granule_pos);
			page_data.push_back(packet_data);
		}
	}
	if (initialized_stream) {
		ogg_stream_clear(&stream_state);
	}
	ogg_sync_clear(&sync_state);

	// Set decoded stream data all at once.
	ogg_packet_sequence->set_packet_granule_positions(page_granule_positions);
	ogg_packet_sequence->set_packet_data(page_data);

	if (ogg_packet_sequence->get_packet_granule_positions().is_empty()) {
		ERR_FAIL_V_MSG(Ref<AudioStreamOggVorbis>(), "Ogg Vorbis decoding failed. Check that your data is a valid Ogg Vorbis audio stream.");
	}

	ogg_vorbis_stream->set_packet_sequence(ogg_packet_sequence);
	ogg_vorbis_stream->set_loop(false);

	if (r_error) {
		*r_error = OK;
	}
	return ogg_vorbis_stream;
}

void ResourceLoaderOggVorbis::get_recognized_extensions(List<String> *p_extensions) const {
	p_extensions->push_back("ogg");
	p_extensions->push_back("oga");
}

bool ResourceLoaderOggVorbis::handles_type(const String &p_type) const {
	return p_type == "AudioStreamOggVorbis";
}

String ResourceLoaderOggVorbis::get_resource_type(const String &p_path) const {
	String ext = p_path.get_extension().to_lower();
	if (ext == "ogg" || ext == "oga") {
		return "AudioStreamOggVorbis";
	}
	return String();
}

