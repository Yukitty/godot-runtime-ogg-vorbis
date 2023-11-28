#pragma once

#include <godot_cpp/classes/resource_format_loader.hpp>
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/templates/hash_set.hpp>

class ResourceLoaderOggVorbis : public ResourceFormatLoader {
	enum {
		OGG_SYNC_BUFFER_SIZE = 8192,
	};

protected:
	static ResourceLoaderOggVorbis *singleton;

public:
	static ResourceLoaderOggVorbis *get_singleton() { return singleton; }

	virtual Ref<Resource> load(const String &p_path, const String &p_original_path = "", Error *r_error = nullptr, bool p_use_sub_threads = false, float *r_progress = nullptr, CacheMode p_cache_mode = CACHE_MODE_REUSE);
	virtual void get_recognized_extensions(List<String> *p_extensions) const;
	virtual bool handles_type(const String &p_type) const;
	virtual String get_resource_type(const String &p_path) const;

	ResourceLoaderOggVorbis() { singleton = this;  }
};

