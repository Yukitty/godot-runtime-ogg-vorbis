#include <gdextension_interface.h>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/file_access.hpp>
using namespace godot;

#include "resource_loader_ogg_vorbis.h"

Ref<Resource> ResourceLoaderOggVorbis::load(const String &p_path, const String &p_original_path, Error *r_error, bool p_use_sub_threads, float *r_progress, CacheMode p_cache_mode) {
	if (r_error) {
		*r_error = ERR_CANT_OPEN;
	}

	Error err;

	Ref<FileAccess> f = FileAccess::open(p_path, FileAccess::READ);
	err = FileAccess::get_open_error();

	ERR_FAIL_COND_V_MSG(err != OK, Ref<Resource>(), "Cannot open file '" + p_path + "'.");

	*r_error = ERR_BUG;
	return Ref<Resource>();
}

void ResourceLoaderOggVorbis::get_recognized_extensions_for_type(const String &p_type, List<String> *p_extensions) const {
	get_recognized_extensions(p_extensions);
}

void ResourceLoaderOggVorbis::get_recognized_extensions(List<String> *p_extensions) const {
	p_extensions->push_back("ogg");
	p_extensions->push_back("oga");
}

bool ResourceLoaderOggVorbis::handles_type(const String &p_type) const {
	// TODO
	return true;
}

void ResourceLoaderOggVorbis::get_classes_used(const String &p_path, HashSet<StringName> *r_classes) {
	r_classes->insert("AudioStreamOggVorbis");
}

String ResourceLoaderOggVorbis::get_resource_type(const String &p_path) const {
	String ext = p_path.get_extension().to_lower();
	if (ext != "ogg" && ext != "oga") {
		return String();
	}
	return "AudioStreamOggVorbis";
}

String ResourceLoaderOggVorbis::get_resource_script_class(const String &p_path) const {
	String ext = p_path.get_extension().to_lower();
	if (ext != "ogg" && ext != "oga") {
		return String();
	}
	return "AudioStreamOggVorbis";
}

int64_t ResourceLoaderOggVorbis::get_resource_uid(const String &p_path) const {
	return ResourceUID::INVALID_ID;
}

void ResourceLoaderOggVorbis::get_dependencies(const String &p_path, List<String> *p_dependencies, bool p_add_types) {
	// No dependencies.
}

Error ResourceLoaderOggVorbis::rename_dependencies(const String &p_path, const HashMap<String, String> &p_map) {
	// No dependencies.
	return OK;
}

ResourceLoaderOggVorbis *ResourceLoaderOggVorbis::singleton = nullptr;

Error ResourceLoaderOggVorbis::convert_file_to_binary(const String &p_src_path, const String &p_dst_path) {
	// Is this even a thing??
	return ERR_BUG;
}

