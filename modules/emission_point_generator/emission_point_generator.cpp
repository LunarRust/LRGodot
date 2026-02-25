#include "emission_point_generator.h"
#include "scene/resources/image_texture.h"
#include "scene/main/node.h"
#include "scene/main/window.h"
// #include "scene/3d/node_3d.h"

#include "core/io/resource_loader.h"
// #include "editor/editor_node.h"
// #include "editor/editor_log.h"

#include "scene/3d/mesh_instance_3d.h"
#include "scene/resources/particle_process_material.h"

void EmissionPointGenerator::_bind_methods() {
	BIND_ENUM_CONSTANT(SURFACE_POINTS);
	BIND_ENUM_CONSTANT(SURFACE_POINTS_AND_NORMAL_DIRECTED);
	BIND_ENUM_CONSTANT(VOLUME);

	ClassDB::bind_method(D_METHOD("generate_emission_points", "emission_source", "emissor_count"), &EmissionPointGenerator::generate_emission_points);
	ClassDB::bind_method(D_METHOD("select_mesh", "p_path"), &EmissionPointGenerator::select_mesh);
	ClassDB::bind_method(D_METHOD("select_particles", "p_path"), &EmissionPointGenerator::select_particles);
	
}

EmissionPointGenerator::EmissionPointGenerator() {
	node = nullptr;
}

void EmissionPointGenerator::select_particles(const NodePath &p_path) {
	GPUParticles3D *casted_node = dynamic_cast<GPUParticles3D*>(SceneTree::get_singleton()->get_root()->get_node_or_null(p_path));
	base_node = casted_node;
	node = casted_node;
}

bool EmissionPointGenerator::_generate(
	Vector<Vector3> &points,
	Vector<Vector3> &normals,
	int emission_source,
	int emissor_count
	) {
	bool use_normals = true;
    // surface points or surface points + normals
	if (emission_source != EmissionSources::VOLUME) {
		float area_accum = 0;
		RBMap<float, int> triangle_area_map;

		for (int i = 0; i < geometry.size(); i++) {
			float area = geometry[i].get_area();
			if (area < CMP_EPSILON) {
				continue;
			}
			triangle_area_map[area_accum] = i;
			area_accum += area;
		}

		if (!triangle_area_map.size() || area_accum == 0) {
			return false;
		}

		for (int i = 0; i < emissor_count; i++) {
			float areapos = Math::random(0.0f, area_accum);

			RBMap<float, int>::Iterator E = triangle_area_map.find_closest(areapos);
			ERR_FAIL_COND_V(!E, false);
			int index = E->value;
			ERR_FAIL_INDEX_V(index, geometry.size(), false);

			// ok FINALLY get face
			Face3 face = geometry[index];
			//now compute some position inside the face...

			Vector3 pos = face.get_random_point_inside();

			points.push_back(pos);

			if (use_normals) {
				Vector3 normal = face.get_plane().normal;
				normals.push_back(normal);
			}
		}
	} else {
		int gcount = geometry.size();

		if (gcount == 0) {
			return false;
		}

		const Face3 *r = geometry.ptr();

		AABB aabb;

		for (int i = 0; i < gcount; i++) {
			for (int j = 0; j < 3; j++) {
				if (i == 0 && j == 0) {
					aabb.position = r[i].vertex[j];
				} else {
					aabb.expand_to(r[i].vertex[j]);
				}
			}
		}

		for (int i = 0; i < emissor_count; i++) {
			int attempts = 5;

			for (int j = 0; j < attempts; j++) {
				Vector3 dir;
				dir[Math::rand() % 3] = 1.0;
				Vector3 ofs = (Vector3(1, 1, 1) - dir) * Vector3(Math::randf(), Math::randf(), Math::randf()) * aabb.size + aabb.position;

				Vector3 ofsv = ofs + aabb.size * dir;

				//space it a little
				ofs -= dir;
				ofsv += dir;

				float max = -1e7, min = 1e7;

				for (int k = 0; k < gcount; k++) {
					const Face3 &f3 = r[k];

					Vector3 res;
					if (f3.intersects_segment(ofs, ofsv, &res)) {
						res -= ofs;
						float d = dir.dot(res);

						if (d < min) {
							min = d;
						}
						if (d > max) {
							max = d;
						}
					}
				}

				if (max < min) {
					continue; //lost attempt
				}

				float val = min + (max - min) * Math::randf();

				Vector3 point = ofs + dir * val;

				points.push_back(point);
				break;
			}
		}
	}

	return true;
}

void EmissionPointGenerator::select_mesh(const NodePath &p_path) {
	Node *sel = SceneTree::get_singleton()->get_root()->get_node_or_null(p_path);
	if (!sel) {
		return;
	}

	if (!sel->is_class("Node3D")) {
		return;
	}

	MeshInstance3D *mi = Object::cast_to<MeshInstance3D>(sel);
	if (!mi || mi->get_mesh().is_null()) {
		return;
	}

	geometry = mi->get_mesh()->get_faces();

	if (geometry.size() == 0) {
		return;
	}

	Transform3D geom_xform = base_node->get_global_transform().affine_inverse() * mi->get_global_transform();

	int gc = geometry.size();
	Face3 *w = geometry.ptrw();

	for (int i = 0; i < gc; i++) {
		for (int j = 0; j < 3; j++) {
			w[i].vertex[j] = geom_xform.xform(w[i].vertex[j]);
		}
	}

	// emission_dialog->popup_centered(Size2(300, 130));
}

void EmissionPointGenerator::generate_emission_points(int emission_source, int emissor_count) {
	Vector<Vector3> points;
	Vector<Vector3> normals;


	if (!_generate(points, normals, emission_source, emissor_count)) {
		return;
	}

	int point_count = points.size();

	int w = 2048;
	int h = (point_count / 2048) + 1;

	Vector<uint8_t> point_img;
	point_img.resize(w * h * 3 * sizeof(float));

	{
		uint8_t *iw = point_img.ptrw();
		memset(iw, 0, w * h * 3 * sizeof(float));
		const Vector3 *r = points.ptr();
		float *wf = reinterpret_cast<float *>(iw);
		for (int i = 0; i < point_count; i++) {
			wf[i * 3 + 0] = r[i].x;
			wf[i * 3 + 1] = r[i].y;
			wf[i * 3 + 2] = r[i].z;
		}
	}

	Ref<Image> image = memnew(Image(w, h, false, Image::FORMAT_RGBF, point_img));
	Ref<ImageTexture> tex = ImageTexture::create_from_image(image);

	Ref<ParticleProcessMaterial> mat = node->get_process_material();
	ERR_FAIL_COND(mat.is_null());

	if (normals.size() > 0) {
		mat->set_emission_shape(ParticleProcessMaterial::EMISSION_SHAPE_DIRECTED_POINTS);
		mat->set_emission_point_count(point_count);
		mat->set_emission_point_texture(tex);

		Vector<uint8_t> point_img2;
		point_img2.resize(w * h * 3 * sizeof(float));

		{
			uint8_t *iw = point_img2.ptrw();
			memset(iw, 0, w * h * 3 * sizeof(float));
			const Vector3 *r = normals.ptr();
			float *wf = reinterpret_cast<float *>(iw);
			for (int i = 0; i < point_count; i++) {
				wf[i * 3 + 0] = r[i].x;
				wf[i * 3 + 1] = r[i].y;
				wf[i * 3 + 2] = r[i].z;
			}
		}

		Ref<Image> image2 = memnew(Image(w, h, false, Image::FORMAT_RGBF, point_img2));
		mat->set_emission_normal_texture(ImageTexture::create_from_image(image2));
	} else {
		mat->set_emission_shape(ParticleProcessMaterial::EMISSION_SHAPE_POINTS);
		mat->set_emission_point_count(point_count);
		mat->set_emission_point_texture(tex);
	}
}
