#pragma once

#include <godot_cpp/classes/resource_format_loader.hpp>
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/resource_uid.hpp>
#include <godot_cpp/templates/hash_set.hpp>

class ResourceLoaderOggVorbis : public ResourceFormatLoader {
public:
	static ResourceLoaderOggVorbis *singleton;
	virtual Ref<Resource> load(const String &p_path, const String &p_original_path = "", Error *r_error = nullptr, bool p_use_sub_threads = false, float *r_progress = nullptr, CacheMode p_cache_mode = CACHE_MODE_REUSE);
	virtual void get_recognized_extensions_for_type(const String &p_type, List<String> *p_extensions) const;
	virtual void get_recognized_extensions(List<String> *p_extensions) const;
	virtual bool handles_type(const String &p_type) const;
	virtual void get_classes_used(const String &p_path, HashSet<StringName> *r_classes);

	virtual String get_resource_type(const String &p_path) const;
	virtual String get_resource_script_class(const String &p_path) const;
	virtual int64_t get_resource_uid(const String &p_path) const;
	virtual void get_dependencies(const String &p_path, List<String> *p_dependencies, bool p_add_types = false);
	virtual Error rename_dependencies(const String &p_path, const HashMap<String, String> &p_map);

	static Error convert_file_to_binary(const String &p_src_path, const String &p_dst_path);

	ResourceLoaderOggVorbis() { singleton = this; }
};

