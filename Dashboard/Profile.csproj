<template>
  <div class="container">
    <!-- Header -->
    <header class="header">
      <div class="logo">👤 <span>User Profile</span></div>
    </header>

    <!-- Profile Summary -->
    <section class="card profile-summary gradient">
      <div class="avatar-wrapper">
        <img
          v-if="profile.avatar"
          :src="profile.avatar"
          class="avatar"
          alt="avatar"
        />
        <div v-else class="avatar-placeholder">🧑</div>

        <!-- Upload Button -->
        <label class="upload-btn">
          📸
          <input type="file" accept="image/*" @change="onAvatarChange" />
        </label>
      </div>

      <div class="info">
        <h2>{{ profile.name || "Your Name" }}</h2>
        <p class="email">{{ profile.email || "example@email.com" }}</p>
      </div>
    </section>

    <!-- Editable Form -->
    <section class="card profile-form">
      <h3>📝 Profile Information</h3>

      <div class="form-grid">
        <div class="form-group">
          <label>Full Name</label>
          <input type="text" v-model="profile.name" placeholder="Enter your full name" />
        </div>

        <div class="form-group">
          <label>Email</label>
          <input type="email" v-model="profile.email" placeholder="example@email.com" />
        </div>

        <div class="form-group">
          <label>Phone Number</label>
          <input type="tel" v-model="profile.phone" placeholder="+66 8x-xxx-xxxx" />
        </div>

        <div class="form-group">
          <label>Address</label>
          <textarea v-model="profile.address" placeholder="Enter your address"></textarea>
        </div>
      </div>

      <div class="btn-row">
        <button class="btn save" @click="saveProfile">💾 Save</button>
        <button class="btn reset" @click="resetProfile">↩ Reset</button>
      </div>
    </section>
  </div>
</template>

<script>
  export default {
  data() {
    return {
      profile: {
        name: "",
        email: "",
        phone: "",
        address: "",
        avatar: "", // 🔹 เพิ่มช่องเก็บรูปโปรไฟล์
      },
    };
  },

  mounted() {
    console.log("🟢 User Profile UI ready");

    // ✅ Watch msg จาก Node-RED (เหมือน Alert / History)
    this.$watch(
      "msg",
      (newMsg) => {
        console.log("📩 Full msg received:", newMsg);
        const payload =
          newMsg?.payload?.profile
            ? newMsg.payload
            : newMsg?.payload?.payload || {};

        if (payload.profile) {
          console.log("✅ Updating profile:", payload.profile);
          this.profile = { ...this.profile, ...payload.profile };
        } else {
          console.warn("⚠️ No profile object found in payload", payload);
        }
      },
      { immediate: true, deep: true }
    );
  },

  methods: {
    saveProfile() {
      console.log("💾 Saving profile:", this.profile);
      this.send({ payload: { profile: this.profile } });
    },
    resetProfile() {
      this.profile = { name: "", email: "", phone: "", address: "", avatar: "" };
    },

    // 📸 เมื่ออัปโหลดรูปใหม่
    onAvatarChange(event) {
      const file = event.target.files[0];
      if (!file) return;
      const reader = new FileReader();
      reader.onload = (e) => {
        this.profile.avatar = e.target.result;
        console.log("🖼️ New avatar loaded");
      };
      reader.readAsDataURL(file);
    },
  },
};
</script>

<style scoped>
  @import url("https://fonts.googleapis.com/css2?family=Inter:wght@400;600&display=swap");

  * {
    font-family: "Inter", sans-serif;
    box-sizing: border-box;
  }

  .container {
    max-width: 900px;
    margin: auto;
    padding: 25px 35px;
    background: #ffffff;
    border-radius: 20px;
    box-shadow: 0 3px 10px rgba(0, 0, 0, 0.05);
  }

  .header {
    display: flex;
    align-items: center;
    margin-bottom: 20px;
  }

  .logo {
    font-size: 28px;
    font-weight: bold;
    color: #2ecc71;
    display: flex;
    align-items: center;
    gap: 8px;
  }

  .card {
    background: #fff;
    border-radius: 15px;
    padding: 20px 25px;
    margin-bottom: 20px;
    box-shadow: 0 2px 8px rgba(0, 0, 0, 0.05);
  }

  .gradient {
    background: linear-gradient(to right, #e8f8f5, #f9fdf8);
  }

  /* ---------- Avatar ---------- */
  .avatar-wrapper {
    position: relative;
    display: flex;
    align-items: center;
    justify-content: center;
  }

  .avatar,
  .avatar-placeholder {
    width: 90px;
    height: 90px;
    border-radius: 50%;
    object-fit: cover;
    background: #f0f0f0;
    display: flex;
    align-items: center;
    justify-content: center;
    font-size: 50px;
  }

  .upload-btn {
    position: absolute;
    bottom: 0;
    right: 0;
    background: #2ecc71;
    color: white;
    border-radius: 50%;
    width: 28px;
    height: 28px;
    display: flex;
    align-items: center;
    justify-content: center;
    cursor: pointer;
    font-size: 14px;
    border: 2px solid #fff;
  }

  .upload-btn input {
    display: none;
  }

  /* ---------- Profile Info ---------- */
  .profile-summary {
    display: flex;
    align-items: center;
    gap: 20px;
  }

  .info h2 {
    margin: 0;
    font-size: 22px;
  }

  .info .email {
    color: #777;
    font-size: 15px;
  }

  /* ---------- Form ---------- */
  .profile-form h3 {
    margin-bottom: 15px;
    font-weight: 600;
    color: #333;
  }

  .form-grid {
    display: grid;
    grid-template-columns: 1fr 1fr;
    gap: 20px;
  }

  .form-group {
    display: flex;
    flex-direction: column;
  }

  label {
    font-weight: 600;
    margin-bottom: 5px;
  }

  input,
  textarea {
    border: 1px solid #ddd;
    border-radius: 10px;
    padding: 10px 12px;
    font-size: 15px;
    outline: none;
    transition: 0.2s;
  }

  textarea {
    resize: vertical;
    min-height: 80px;
  }

  input:focus,
  textarea:focus {
    border-color: #2ecc71;
    box-shadow: 0 0 5px rgba(46, 204, 113, 0.3);
  }

  /* ---------- Buttons ---------- */
  .btn-row {
    margin-top: 25px;
    display: flex;
    gap: 15px;
    justify-content: flex-end;
  }

  .btn {
    border: none;
    padding: 10px 18px;
    border-radius: 10px;
    cursor: pointer;
    font-weight: 600;
    transition: 0.2s;
  }

  .btn.save {
    background: #2ecc71;
    color: white;
  }

  .btn.reset {
    background: #e0e0e0;
  }

  .btn.save:hover {
    background: #27ae60;
  }

  .btn.reset:hover {
    background: #d6d6d6;
  }

  /* ---------- Responsive ---------- */
  @media (max-width: 600px) {
    .form-grid {
      grid-template-columns: 1fr;
    }

    .container {
      padding: 15px;
    }

    .logo {
      font-size: 22px;
    }
  }
</style>
